#ifndef TCP_NETWORK_H
#define TCP_NETWORK_H
#include "tcp_types.h"
#include "tcp_epoll.h"
int init_socket(struct tcp_state *state);
int accept_new_client(int tcp_fd, struct tcp_state *state);
void handle_client_event(int client_fd, uint32_t revents,
                                struct tcp_state *state);
                                
#endif
