#ifndef TCP_CONSTANTS_H
#define TCP_CONSTANTS_H

#define PRERF "(errno=%d) %s\n"
#define PREAR(NUM) NUM, strerror(NUM)
#define EPOLL_MAP_TO_NOP (0u)
#define EPOLL_MAP_SHIFT  (1u) /* Shift to cover reserved value MAP_TO_NOP */

#endif