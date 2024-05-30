#include "tcp_network.h"

int accept_new_client(int tcp_fd, struct tcp_state *state)
{
    int err;
    int client_fd;
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    uint16_t src_port;
    const char *src_ip;
    char src_ip_buf[sizeof("xxx.xxx.xxx.xxx")];
    const size_t client_slot_num = sizeof(state->clients) / sizeof(*state->clients);


    memset(&addr, 0, sizeof(addr));
    client_fd = accept(tcp_fd, (struct sockaddr *)&addr, &addr_len);
    if (client_fd < 0) {
        err = errno;
        if (err == EAGAIN)
            return 0;

        /* Error */
        printf("accept(): " PRERF, PREAR(err));
        return -1;
    }

    src_port = ntohs(addr.sin_port);
    src_ip   = convert_addr_ntop(&addr, src_ip_buf);
    if (!src_ip) {
        printf("Cannot parse source address\n");
        goto out_close;
    }


    /*
     * Find unused client slot.
     *
     * In real world application, you don't want to iterate
     * the whole array, instead you can use stack data structure
     * to retrieve unused index in O(1).
     *
     */
    for (size_t i = 0; i < client_slot_num; i++) {
        struct client_slot *client = &state->clients[i];

        if (!client->is_used) {
            /*
             * We found unused slot.
             */

            client->client_fd = client_fd;
            memcpy(client->src_ip, src_ip_buf, sizeof(src_ip_buf));
            client->src_port = src_port;
            client->is_used = true;
            client->my_index = i;

            /*
             * We map the client_fd to client array index that we accept
             * here.
             */
            state->client_map[client_fd] = client->my_index + EPOLL_MAP_SHIFT;

            /*
             * Let's tell to `epoll` to monitor this client file descriptor.
             */
            my_epoll_add(state->epoll_fd, client_fd, EPOLLIN | EPOLLPRI);

            //printf("Client %s:%u has been accepted!\n", src_ip, src_port);
            printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>");
            printf("Client :%u has been accepted!\n", src_port);
            return 0;
        }
    }
    printf("Sorry, can't accept more client at the moment, slot is full\n");


out_close:
    close(client_fd);
    return 0;
}

void handle_client_event(int client_fd, uint32_t revents,
                                struct tcp_state *state)
{
    int err;
    ssize_t recv_ret;
    char buffer[1024];
    const uint32_t err_mask = EPOLLERR | EPOLLHUP;
    /*
     * Read the mapped value to get client index.
     */
    uint32_t index = state->client_map[client_fd] - EPOLL_MAP_SHIFT;
    struct client_slot *client = &state->clients[index];

    if (revents & err_mask)
        goto close_conn;

    recv_ret = recv(client_fd, buffer, sizeof(buffer), 0);
    if (recv_ret == 0)
        goto close_conn;

    if (recv_ret < 0) {
        err = errno;
        if (err == EAGAIN)
            return;

        /* Error */
        printf("recv(): " PRERF, PREAR(err));
        goto close_conn;
    }


    /*
     * Safe printing
     */
    buffer[recv_ret] = '\0';
    if (buffer[recv_ret - 1] == '\n') {
        buffer[recv_ret - 1] = '\0';
    }

    printf("Client %s:%u sends: \"%s\"\n", client->src_ip, client->src_port,
           buffer);
    return;


close_conn:
    printf("Client %s:%u has closed its connection\n", client->src_ip,
           client->src_port);
    my_epoll_delete(state->epoll_fd, client_fd);
    close(client_fd);
    client->is_used = false;
    return;
}

int init_socket(struct tcp_state *state)
{
    int ret;
    int err;
    int tcp_fd = -1;
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    const char *bind_addr = "0.0.0.0";
    uint16_t bind_port = 1234;

    printf("Creating TCP socket...\n");
    tcp_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, IPPROTO_TCP);
    if (tcp_fd < 0) {
        err = errno;
        printf("socket(): " PRERF, PREAR(err));
        return -1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(bind_port);
    addr.sin_addr.s_addr = inet_addr(bind_addr);

    ret = bind(tcp_fd, (struct sockaddr *)&addr, addr_len);
    if (ret < 0) {
        ret = -1;
        err = errno;
        printf("bind(): " PRERF, PREAR(err));
        goto out;
    }

    ret = listen(tcp_fd, 10);
    if (ret < 0) {
        ret = -1;
        err = errno;
        printf("listen(): " PRERF, PREAR(err));
        goto out;
    }

    /*
     * Add `tcp_fd` to epoll monitoring.
     *
     * If epoll returned tcp_fd in `events` then a client is
     * trying to connect to us.
     */
    ret = my_epoll_add(state->epoll_fd, tcp_fd, EPOLLIN | EPOLLPRI);
    if (ret < 0) {
        ret = -1;
        goto out;
    }

    printf("Listening on %s:%u...\n", bind_addr, bind_port);
    state->tcp_fd = tcp_fd;
    return 0;
out:
    close(tcp_fd);
    return ret;
}
