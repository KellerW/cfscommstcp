#include "tcp_epoll.h"

int my_epoll_add(int epoll_fd, int fd, uint32_t events)
{
    int err;
    struct epoll_event event;

    /* Shut the valgrind up! */
    memset(&event, 0, sizeof(struct epoll_event));

    event.events  = events;
    event.data.fd = fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event) < 0) {
        err = errno;
        printf("epoll_ctl(EPOLL_CTL_ADD): " PRERF, PREAR(err));
        return -1;
    }
    return 0;
}



int my_epoll_delete(int epoll_fd, int fd)
{
    int err;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL) < 0) {
        err = errno;
        printf("epoll_ctl(EPOLL_CTL_DEL): " PRERF, PREAR(err));
        return -1;
    }
    return 0;
}
