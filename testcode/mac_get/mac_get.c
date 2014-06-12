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
#include <db_185.h>
#include <linux/if_ether.h>

typedef unsigned char mac_t[6];  //mac address type

int get_if_ethaddr(char *devname, mac_t mac)
{
    int s, err;
    struct ifreq req;

    s = socket(PF_INET,SOCK_DGRAM,0);
    strcpy(req.ifr_name, devname);

    err = ioctl(s, SIOCGIFHWADDR, &req);

    close(s);

    if(err == -1)
    {
        return 1;
    }

    memcpy(mac, req.ifr_hwaddr.sa_data, ETH_ALEN);

    return 0;
}

void
mac_string(mac_t mac, char *str)
{
    sprintf(str, "%.2x:%.2x:%.2x:%.2x:%.2x:%.2x",
            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}


int get_gw_ip(char *eth, char *ipaddr)
{
 int sock_fd;
 struct  sockaddr_in my_addr;
 struct ifreq ifr;

 /**//* Get socket file descriptor */
 if ((sock_fd = socket(PF_INET, SOCK_DGRAM, 0)) == -1)
 {
  perror("socket");
  return RET_ERROR;
 }

 /**//* Get IP Address */
 strncpy(ifr.ifr_name, eth, 16);
 //ifr.ifr_name[IFNAMSIZ-1]='/0';

 if (ioctl(sock_fd, SIOCGIFADDR, &ifr) < 0)
 {
  printf(":No Such Device %s/n",eth);
  return RET_ERROR;
 }

 memcpy(&my_addr, &ifr.ifr_addr, sizeof(my_addr));
 strcpy(ipaddr, inet_ntoa(my_addr.sin_addr));
 close(sock_fd);
 return RET_SUCCESS;
}

int main(int argc, char *argv[])
{
    char ipaddr[25];
    mac_t mac;
    char str[64];
    if (argc < 2)
    {
        printf("please input like this:\"./mag_get eth0\"\n");
        return 1;
    }
    memset(ipaddr, 0, sizeof(ipaddr));
    memset(mac,0,sizeof(mac));
    memset(str,0,sizeof(str));
    get_if_ethaddr(argv[1], mac);
    mac_string(mac, str);
    get_gw_ip(argv[1], ipaddr);
    printf("%s: mac:%s ",argv[1], str);
    printf("ipaddr:%s\n",ipaddr);
    return 0;

}
