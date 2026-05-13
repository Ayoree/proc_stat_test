#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <assert.h>

#define ALL_CORES_ID USHRT_MAX // Let's assume we will never hit `USHRT_MAX` cores, so `USHRT_MAX` will represent total percent for all cores

typedef unsigned long long ull_t;

typedef struct {
    unsigned short core_num;
    double percent;
} udp_client_message_t;
