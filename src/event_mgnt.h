#ifndef EVENT_MGNT_H
#define EVENT_MGNT_H

#include "tcp_types.h"  
#include "tcp_network.h"

int event_loop(struct tcp_state *state);
int init_epoll(struct tcp_state *state);

#endif // EVENT_MANAGEMENT_H