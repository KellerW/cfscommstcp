#ifndef TCP_TYPES_H
#define TCP_TYPES_H

#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <unistd.h>
#include "tcp_constants.h"


#define PRERF "(errno=%d) %s\n"
#define PREAR(NUM) NUM, strerror(NUM)

struct client_slot {
    bool is_used;
    int client_fd;
    char src_ip[sizeof("xxx.xxx.xxx.xxx")];
    uint16_t src_port;
    uint16_t my_index;
};

struct tcp_state {
    bool stop;
    int tcp_fd;
    int epoll_fd;
    uint16_t client_c;
    struct client_slot clients[10];
    uint32_t client_map[10000]; // Assuming no FD > 10000
};
#endif
