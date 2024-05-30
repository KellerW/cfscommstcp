#include "utils.h"
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