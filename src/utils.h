#ifndef UTILS_H
#define UTILS_H
#include "tcp_types.h"
#include <stdio.h>
#include <stdlib.h>

bool can_send_data(struct tcp_state *state); 
int read_config_port(const char* pfile);
const char *convert_addr_ntop(struct sockaddr_in *addr, char *src_ip_buf);
#endif