#include <stdio.h>
#include "tcp_epoll.h"
#include "event_mgnt.h"
#include "tcp_types.h"
#include "tcp_network.h"
#include "state_mgnt.h"
#include "utils.h"
#include "tcp_constants.h"
#include "state_mgnt.h"
int main (int argc, char* argv)
{
       
    int ret;
    struct tcp_state state;

    const char* config_file = "/home/wagner/work/cfscommstcp/config.txt";
    int port = read_config_port(config_file);
    if (port > 0) {
        printf("Port number read from file: %d\n", port);
    } else {
        printf("Failed to read a valid port number from the file\n");
    }
    
    init_state(&state);

    ret = init_epoll(&state);
    if (ret != 0)
        goto out;

    ret = init_socket(&state);
    if (ret != 0)
        goto out;

    state.stop = false;

    ret = event_loop(&state);

out:
    /*
     * You should write a cleaner here.
     *
     * Close all client file descriptors and release
     * some resources you may have.
     *
     * You may also want to set interrupt handler
     * before the event_loop.
     *
     * For example, if you get SIGINT or SIGTERM
     * set `state->stop` to true, so that it exits
     * gracefully.
     */
    return ret;
    
}