#include "utils.h"


bool can_send_data(struct tcp_state *state) 
{
    // Check if there's data in the buffer and if the socket is marked as ready to send
    return strlen(state->data) > 0 && state->is_ready_to_send;
}

const char *convert_addr_ntop(struct sockaddr_in *addr, char *src_ip_buf)
{
    int err;
        const char *ret;
        in_addr_t saddr = addr->sin_addr.s_addr;

        ret = inet_ntop(AF_INET, &saddr, src_ip_buf, sizeof("xxx.xxx.xxx.xxx"));
        if (ret == NULL) {
            err = errno;
            err = err ? err : EINVAL;
            printf("inet_ntop(): " PRERF, PREAR(err));
            return NULL;
        }

        return ret;
}

int read_config_port(const char* pfile) 
{
    FILE *file;
    char buffer[256]; // Buffer to hold the read string, assuming the port number won't exceed this length
    int port;

    // Open the file
    file = fopen(pfile, "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1; // Return an error code if file cannot be opened
    }

    // Read the first line from the file
    if (fgets(buffer, sizeof(buffer), file) == NULL) {
        perror("Error reading from file");
        fclose(file);
        return -1; // Return an error code if reading fails
    }

    // Convert the read string to an integer
    port = atoi(buffer);
    if (port <= 0) {
        fprintf(stderr, "Invalid port number read from file\n");
        fclose(file);
        return -1; // Return an error code if the conversion results in an invalid port
    }

    // Close the file
    fclose(file);

    // Return the read and converted port number
    return port;
}