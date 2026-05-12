#include "proc_stat.h"

ull_t cpu_total(const cpu_stat_t* s)
{
    return s->user + s->nice + s->system + s->idle + s->iowait + s->irq + s->softirq + s->steal;
}

snapshot_t read_stat(FILE* fd)
{
    snapshot_t snap = {
        .num_cores = 0
    };
    char line[256];
    unsigned int coreIndex;

    while (fgets(line, sizeof(line), fd)) {
        if (strncmp(line, "cpu ", 4) == 0) {
            const int read = sscanf(line, "cpu  %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu",
                &snap.total.user,
                &snap.total.nice,
                &snap.total.system,
                &snap.total.idle,
                &snap.total.iowait,
                &snap.total.irq,
                &snap.total.softirq,
                &snap.total.steal,
                &snap.total.guest,
                &snap.total.guest_nice
            );
            if (read != 10)
            {
                perror("Sorry, but I'm too lazy to implement support for old linux versions like yours.");
                exit(EXIT_FAILURE);
            }
            continue;
        }
        if (strncmp(line, "cpu", 3) == 0 && line[3] >= '0' && line[3] <= '9') {
            if (snap.num_cores >= PROC_STAT_MAX_CPUS)
                continue;

            coreIndex = snap.num_cores++;
            sscanf(line, "cpu%*d %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu",
                &snap.core[coreIndex].user,
                &snap.core[coreIndex].nice,
                &snap.core[coreIndex].system,
                &snap.core[coreIndex].idle,
                &snap.core[coreIndex].iowait,
                &snap.core[coreIndex].irq,
                &snap.core[coreIndex].softirq,
                &snap.core[coreIndex].steal,
                &snap.core[coreIndex].guest,
                &snap.core[coreIndex].guest_nice
            );
            continue;
        }
        if (snap.num_cores > 0 && strncmp(line, "cpu", 3) != 0)
            break;
    }
    rewind(fd);
    fflush(fd);
    
    return snap;
}

double get_idle_percent(const cpu_stat_t* prev, const cpu_stat_t* curr)
{
    unsigned long long delta_idle  = curr->idle - prev->idle;
    unsigned long long delta_total = cpu_total(curr) - cpu_total(prev);

    if (delta_total == 0)
        return 1.0;
    
    return (double)delta_idle / (double)delta_total;
}

double get_busy_percent(const cpu_stat_t *prev, const cpu_stat_t *curr)
{
    return 1.0 - get_idle_percent(prev, curr);
}