#include "udp_client.h"

int udp_client_create_socket(const char* addr, unsigned short port)
{
    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_addr.s_addr = inet_addr(addr);
    servaddr.sin_port = htons(port);
    servaddr.sin_family = AF_INET;

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1)
    {
        perror("Can't create socket");
        exit(EXIT_FAILURE);
    }
    assert(connect(sockfd, (void*)&servaddr, sizeof(servaddr)) == 0); // Who cares about UDP connection errors?

    return sockfd;
}

void udp_client_send_total(int sockfd, const double percent)
{
    udp_client_send_core(sockfd, ALL_CORES_ID, percent);
}

void udp_client_send_core(int sockfd, const unsigned short core_num, const double percent)
{
    udp_client_message_t msg = {
        .core_num = core_num,
        .percent = percent
    };

    if (send(sockfd, &msg, sizeof(msg), 0) == -1)
        perror("Can't send data through socket");
}