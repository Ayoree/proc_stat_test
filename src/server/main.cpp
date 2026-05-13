#include <QApplication>
#include "graphwindow.hpp"

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

    QApplication a(argc, argv);
    GraphWindow w(port);
    w.show();
    
    return a.exec();
}
