#pragma once

inline int udp_server_create_socket(const unsigned short port)
{
    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(port);
    servaddr.sin_family = AF_INET;

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1)
    {
        perror("Can't create socket");
        exit(EXIT_FAILURE);
    }
    if (bind(sockfd, (const struct sockaddr*)&servaddr, sizeof(servaddr)) == -1)
    {
        perror("Can't bind socket");
        exit(EXIT_FAILURE);
    }

    return sockfd;
}