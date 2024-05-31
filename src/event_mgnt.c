#include "event_mgnt.h"

int event_loop(struct tcp_state *state)
{
    int err;
    int ret = 0;
    int timeout = 3000; /* in milliseconds */
    int maxevents = 32;
    int epoll_ret;
    int epoll_fd = state->epoll_fd;
    struct epoll_event events[maxevents];

    printf("Entering event loop...\n");

    while (!state->stop) 
    {
        int epoll_ret = epoll_wait(epoll_fd, events, maxevents, timeout);

        if (epoll_ret == 0) 
        {
            /*
             *`epoll_wait` reached its timeout
             */
            printf("I don't see any event within %d milliseconds\n", timeout);
            continue;
        }
        if (epoll_ret == -1) 
        {
            err = errno;
            switch (err) 
            {
            case EINTR:
            // Log more information about the interrupt
                printf("epoll_wait was interrupted by a signal. No need to stop the loop.\n");
                continue;
            case EBADF:
            // Fatal error: epoll file descriptor is not valid
                fprintf(stderr, "Fatal error: Invalid file descriptor for epoll_wait. Stopping server.\n");
                return -1; // Consider stopping the event loop or restarting components
            case EFAULT:
            // Serious error: The memory area pointed to by events is not accessible
                fprintf(stderr, "Fatal error: Memory access error with epoll_wait.\n");
                return -1;
            case EINVAL:
            // This should never happen if the event loop is set up correctly
                fprintf(stderr, "Fatal error: Invalid epoll or maxevents argument.\n");
                return -1;
            default:
            // Log unexpected errors
                fprintf(stderr, "Unexpected epoll_wait error: %s\n", strerror(err));
                return -1;
            }
        }
        for (int i = 0; i < epoll_ret; i++) 
        {
            int fd = events[i].data.fd;
            if (fd == state->tcp_fd)
            {
                /*
                 * A new client is connecting to us...
                 */
                if (accept_new_client(fd, state) < 0) 
                {
                    ret = -1;
                    goto out;
                }
                 continue;
            }
            if (events[i].events & (uint32_t)EPOLLIN) 
            {
                handle_receive(events[i].data.fd, state);
            }
            if (events[i].events & (uint32_t)EPOLLOUT) 
            {
              //  printf("teste2");
                handle_send(events[i].data.fd, state);
            }
            if (events[i].events && (uint32_t)EPOLLERR) 
            {
              //  handle_error(events[i].data.fd, state);
            }
        }
    }

out:
    //close_all_client_sockets(state);
    return ret;
}

int init_epoll(struct tcp_state *state)
{
    int err;
    int epoll_fd;

    printf("Initializing epoll_fd...\n");

    /* The epoll_create argument is ignored on modern Linux */
    epoll_fd = epoll_create(255);
    if (epoll_fd < 0) 
    {
        err = errno;
        printf("epoll_create(): " PRERF, PREAR(err));
        return -1;
    }

    state->epoll_fd = epoll_fd;
    return 0;
}

// Function to check for unsent data and update epoll interests
void check_and_update_epoll_interests(int epoll_fd, int sock_fd, struct tcp_state *state) 
{
    //if (has_unsent_data(&state)) 
    // {
    //     update_epoll_interest(epoll_fd, sock_fd, EPOLLIN | EPOLLPRI | EPOLLOUT);
    // } 
    //else 
    // {
    //     update_epoll_interest(epoll_fd, sock_fd, EPOLLIN | EPOLLPRI);
    // }
}

