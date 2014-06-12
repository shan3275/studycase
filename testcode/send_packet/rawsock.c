

#include "rawsock.h"


int rawsock_close(rawsock_desc_t *rawsock)
{
    return close(rawsock->fd);
}

int rawsock_open(rawsock_desc_t *rawsock, char *devname, unsigned short protocol)
{
    int ret;
    struct ifreq req;
    int sd;

    rawsock->fd = socket(PF_PACKET, SOCK_RAW, htons(protocol));
    if(rawsock->fd < 0)
    {
        return 1;
    }

    rawsock->devname = strdup(devname);
    rawsock->protocol = protocol;

    rawsock->stTagAddr.sll_family    = AF_PACKET;
    rawsock->stTagAddr.sll_protocol  = htons(rawsock->protocol);

    sd = socket(PF_INET,SOCK_DGRAM,0);
    strcpy(req.ifr_name, rawsock->devname);
    ret = ioctl(sd, SIOCGIFINDEX, &req);

    close(sd);
    if (ret == -1)
    {
       RAWSOCK_ERROR("Get %s index error.\n", rawsock->devname);
       return 1;
    }

    rawsock->stTagAddr.sll_ifindex   = req.ifr_ifindex;
    rawsock->stTagAddr.sll_pkttype   = PACKET_OUTGOING;
    rawsock->stTagAddr.sll_halen     = 6;

    return 0;
}


int rawsock_recv(rawsock_desc_t *rawsock, char *pkt_buff)
{
    return recvfrom(rawsock->fd, pkt_buff, RAWSOCKET_PKT_BUFF_LEN, 0, NULL, NULL);
}

int rawsock_send(rawsock_desc_t *rawsock,  char *pkt_buff, uint16_t pkt_len)
{
    return sendto(rawsock->fd, pkt_buff, pkt_len, 0, (const struct sockaddr *)&rawsock->stTagAddr, sizeof(rawsock->stTagAddr));
}

/* End of file */

