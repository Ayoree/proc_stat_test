#include "proc_stat/proc_stat.h"
#include "udp_client.h"

const char* addr = "127.0.0.1";
unsigned short port = 1234;

FILE* proc_stat_fd;

int main(int argc, const char *argv[])
{
    if (argc == 1)
        printf("Arguments is not specified. Using default values. (try `-h` for usage)\n");
    
    if (argc >= 2 && strcmp(argv[1], "-h") == 0)
    {
        printf("Usage: [address] [port]\n");
        return EXIT_SUCCESS;
    }

    if (argc >= 2)
        addr = argv[1];
    if (argc >= 3)
        port = atoi(argv[2]);

    printf("Address: %s, port: %u\n", addr, port);

    proc_stat_fd = fopen(PROC_STAT_PATH, "r");
    if (!proc_stat_fd) {
        perror("Can't open: " PROC_STAT_PATH);
        return EXIT_FAILURE;
    }

    int sockfd = udp_client_create_socket(addr, port);
    
    printf("Measuring...\n");
    snapshot_t prev = read_stat(proc_stat_fd);
    for (;;)
    {
        sleep(1);
        const snapshot_t curr = read_stat(proc_stat_fd);
    
        double busyPercent = get_busy_percent(&prev.total, &curr.total);
        printf("Total: %.2f\n", busyPercent);
        udp_client_send_total(sockfd, busyPercent);
        
        for (unsigned short i = 0; i < prev.num_cores && i < curr.num_cores; ++i)
        {
            busyPercent = get_busy_percent(&prev.core[i], &curr.core[i]);
            printf("\tCore %u: %.2f\n", i, busyPercent);
            udp_client_send_core(sockfd, i, busyPercent);
        }
        prev = curr;
    }


    fclose(proc_stat_fd);
    close(sockfd);
    return EXIT_SUCCESS;
}