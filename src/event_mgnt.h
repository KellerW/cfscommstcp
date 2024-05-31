#ifndef EVENT_MGNT_H
#define EVENT_MGNT_H

#include "tcp_types.h"  
#include "tcp_network.h"

int event_loop(struct tcp_state *state);
int init_epoll(struct tcp_state *state);
void check_and_update_epoll_interests(int epoll_fd, int sock_fd, struct tcp_state *state); 
#endif // EVENT_MANAGEMENT_H