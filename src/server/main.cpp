int udp_server_create_socket(const unsigned short port)
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

int main(int argc, const char *argv[])
{
    unsigned short port = 1234;
    if (argc == 1)
        printf("Arguments is not specified. Using default values. (try `-h` for usage)\n");
    
    if (argc >= 2 && strcmp(argv[1], "-h") == 0)
    {
        printf("Usage: [port]\n");
        return EXIT_SUCCESS;
    }

    if (argc >= 2)
        port = atoi(argv[1]);

    printf("Port: %u\n", port);

    int sockfd = udp_server_create_socket(port);

    printf("Waiting for messages...\n");
    udp_client_message_t msg;
    for(;;)
    {
        const int recvRes = recv(sockfd, &msg, sizeof(msg), 0);
        if (recvRes == -1)
            perror("Error while receiving socket data");
        else if (recvRes != sizeof(msg))
            perror("Wrong message format");
        else
        {
            if (msg.core_num == USHRT_MAX)
                printf("Total: %.2f\n", msg.percent);
            else
                printf("\tCore %u: %.2f\n", msg.core_num, msg.percent);
            fflush(stdout);
        }
    }

    return 0;
}