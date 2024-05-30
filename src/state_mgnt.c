#include "state_mgnt.h"
void init_state(struct tcp_state *state)
{
    const size_t client_slot_num = sizeof(state->clients) / sizeof(*state->clients);
    const uint16_t client_map_num = sizeof(state->client_map) / sizeof(*state->client_map);

    for (size_t i = 0; i < client_slot_num; i++) {
        state->clients[i].is_used = false;
        state->clients[i].client_fd = -1;
    }

    for (uint16_t i = 0; i < client_map_num; i++) {
        state->client_map[i] = EPOLL_MAP_TO_NOP;
    }
}