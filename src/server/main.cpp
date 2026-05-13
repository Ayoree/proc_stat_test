#include <QApplication>
#include "udp_server.h"
#include "mainwindow.hpp"

unsigned short port = 1234;

int main(int argc, char *argv[])
{
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

    // int sockfd = udp_server_create_socket(port);

    // printf("Waiting for messages...\n");
    // udp_client_message_t msg;
    // for(;;)
    // {
    //     const int recvRes = recv(sockfd, &msg, sizeof(msg), 0);
    //     if (recvRes == -1)
    //         perror("Error while receiving socket data");
    //     else if (recvRes != sizeof(msg))
    //         perror("Wrong message format");
    //     else
    //     {
    //         if (msg.core_num == USHRT_MAX)
    //             printf("Total: %.2f\n", msg.percent);
    //         else
    //             printf("\tCore %u: %.2f\n", msg.core_num, msg.percent);
    //         fflush(stdout);
    //     }
    // }

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();

    return 0;
}
