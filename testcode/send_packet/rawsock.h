#ifndef __RAWSOCK_H__
#define __RAWSOCK_H__

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <linux/if.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>

#include "stdint.h"

#define RAWSOCKET_PKT_BUFF_LEN  2048
#define ETH_ADDR_LEN             6

typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long UINT64_T;
#define uint64_t UINT64_T

#define ULL unsigned long long

typedef uint8_t mac_t[6];
typedef struct {
    char *devname;
    int fd;
    int protocol;
    struct sockaddr_ll stTagAddr;
} rawsock_desc_t;
#define RAWSOCK_ERROR(_fmt, _args...) printf("%s.%d: [RAWSOCK]" _fmt, __func__, __LINE__, ##_args)
int rawsock_close(rawsock_desc_t *rawsock);
int rawsock_open(rawsock_desc_t *rawsock, char *devname, unsigned short protocol);
int rawsock_recv(rawsock_desc_t *rawsock, char *pkt_buff);
int rawsock_send(rawsock_desc_t *rawsock,  char *pkt_buff, uint16_t pkt_len);

#endif /* !__RAWSOCK_H__ */
/* End of file */

