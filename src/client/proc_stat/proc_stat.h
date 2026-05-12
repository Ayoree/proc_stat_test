#pragma once

#define PROC_STAT_MAX_CPUS 128
#define PROC_STAT_PATH "/proc/stat"

typedef struct {
    ull_t user;
    ull_t nice;
    ull_t system;
    ull_t idle;
    ull_t iowait;
    ull_t irq;
    ull_t softirq;
    ull_t steal;
    ull_t guest;
    ull_t guest_nice;
} cpu_stat_t;
 
typedef struct {
    cpu_stat_t total;
    cpu_stat_t core[PROC_STAT_MAX_CPUS];
    unsigned short num_cores;
} snapshot_t;

// Returns sum of all cpu_stat_t fields, avoids counting them twice (`guest` and `guest_nice` are ignored)
ull_t cpu_total(const cpu_stat_t* s);

// Parse file descriptor to get a snapshot
snapshot_t read_stat(FILE* fd);

// Returns idle percent from `0.0` to `1.0`
double get_idle_percent(const cpu_stat_t* prev, const cpu_stat_t* curr);
// Returns busy percent from `0.0` to `1.0`
double get_busy_percent(const cpu_stat_t* prev, const cpu_stat_t* curr);