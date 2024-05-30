#include "event_mgnt.h"


int event_loop(struct tcp_state *state)
{
    int err;
    int ret = 0;
    int timeout = 3000; /* in milliseconds */
    int maxevents = 32;
    int epoll_ret;
    int epoll_fd = state->epoll_fd;
    struct epoll_event events[32];

    printf("Entering event loop...\n");

    while (!state->stop) {

        /*
         * I sleep on `epoll_wait` and the kernel will wake me up
         * when event comes to my monitored file descriptors, or
         * when the timeout reached.
         */
        epoll_ret = epoll_wait(epoll_fd, events, maxevents, timeout);


        if (epoll_ret == 0) {
            /*
             *`epoll_wait` reached its timeout
             */
            printf("I don't see any event within %d milliseconds\n", timeout);
            continue;
        }


        if (epoll_ret == -1) {
            err = errno;
            if (err == EINTR) {
                printf("Something interrupted me!\n");
                continue;
            }

            /* Error */
            ret = -1;
            printf("epoll_wait(): " PRERF, PREAR(err));
            break;
        }


        for (int i = 0; i < epoll_ret; i++) {
            int fd = events[i].data.fd;

            if (fd == state->tcp_fd) {
                /*
                 * A new client is connecting to us...
                 */
                if (accept_new_client(fd, state) < 0) {
                    ret = -1;
                    goto out;
                }
                continue;
            }

            /*
             * We have event(s) from client, let's call `recv()` to read it.
             */
            handle_client_event(fd, events[i].events, state);
        }
    }

out:
    return ret;
}


int init_epoll(struct tcp_state *state)
{
    int err;
    int epoll_fd;

    printf("Initializing epoll_fd...\n");

    /* The epoll_create argument is ignored on modern Linux */
    epoll_fd = epoll_create(255);
    if (epoll_fd < 0) {
        err = errno;
        printf("epoll_create(): " PRERF, PREAR(err));
        return -1;
    }

    state->epoll_fd = epoll_fd;
    return 0;
}

