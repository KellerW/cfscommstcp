#ifndef TCP_EPOLL_H
#define TCP_EPOLL_H
#include "tcp_types.h"

int my_epoll_add(int epoll_fd, int fd, uint32_t events);
int my_epoll_delete(int epoll_fd, int fd);

#endif