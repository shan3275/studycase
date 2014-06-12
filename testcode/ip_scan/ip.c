/**
 * this file is used to probe ip address. 
 * @author : fangtong<fangtong2009@gmail.com> 
 * @bug  : unknown. 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <arpa/inet.h>

#define ETH_P_ARP 0x0806
#define MAX_PACK_LEN 2000
#define ETHER_HEADER_LEN 14
#define ARPREQUEST 0x0001
#define ARPREPLY 0x0002

#define perr(s)  fprintf(stderr,s)

struct arp_struct
{
      u_char dst_mac[6];
      u_char src_mac[6];
      u_short pkt_type;
      u_short hw_type;
      u_short pro_type;
      u_char hw_len;
      u_char pro_len;
      u_short arp_op;
      u_char sender_eth[6];
      u_char sender_ip[4];
      u_char target_eth[6];
      u_char target_ip[4];
};
union
{
    u_char full_packet[MAX_PACK_LEN];
    struct arp_struct arp_pkt;
}a;

#define full_packet a.full_packet
#define arp_pkt a.arp_pkt

int do_ip_collision_probe(char *ipaddr)
{
	int rec=0;
	int ret=0;
	int len=0, from_len=0, rsflags=0;
	u_int8_t myMAC[6];
	u_int32_t myIP, myNETMASK, myBROADCAST, ip, sip,kip;
	char devname[10] = {0};
	fd_set fdset;
	int retval = 0;
	struct timeval tmout;

	tmout.tv_sec = 0;
	tmout.tv_usec = 15000;
  
	struct ifreq if_data;
	struct sockaddr from;
	struct in_addr testaddr;
	if (NULL == ipaddr){
		return -1;
	}

	ret= inet_aton(ipaddr,&testaddr);

	if(ret==0){
		return -1;
	}

	kip=testaddr.s_addr;

	if(getuid() != 0){
		return -1;
	}

	if((rec = socket(AF_INET, SOCK_PACKET, htons(ETH_P_ARP))) < 0){
		return -1;
	}

#ifdef EMBED
	if (0 == system("ifconfig br0 > /tmp/dev")){
		strcpy (devname, "br0");
	} else {
		strcpy (devname, "eth1");
	}
#else
	strcpy (devname, "eth0");
#endif

	strcpy (if_data.ifr_name, devname);
	if(ioctl(rec, SIOCGIFHWADDR, &if_data) < 0) {
		close(rec);return -1;
	}

	memcpy(myMAC,if_data.ifr_hwaddr.sa_data, 6);

	if(ioctl(rec, SIOCGIFADDR, &if_data) < 0){
		close(rec);return -1;
	}
      

	memcpy((void *)&myIP, (void *)&if_data.ifr_addr.sa_data + 2, 4);

	if(ioctl (rec, SIOCGIFNETMASK, &if_data) < 0){
		close(rec);return -1;
	}

	memcpy((void *) &ip, (void *) &if_data.ifr_netmask.sa_data + 2, 4);
	myNETMASK = ntohl (ip);
	if (ioctl (rec, SIOCGIFBRDADDR, &if_data) < 0) {
			close(rec);return -1;
	}
	memcpy ((void *) &ip, (void *) &if_data.ifr_broadaddr.sa_data + 2, 4);
	myBROADCAST = ntohl (ip);
	if ((rsflags = fcntl (rec, F_GETFL)) == -1){
			close(rec);return -1;
	}
	if (fcntl (rec, F_SETFL, rsflags | O_NONBLOCK) == -1) {
			close(rec);return -1;
	}
	bzero (full_packet, MAX_PACK_LEN);
	memcpy (arp_pkt.dst_mac, "\xff\xff\xff\xff\xff\xff", 6);
	memcpy (arp_pkt.src_mac, myMAC, 6);
	arp_pkt.pkt_type = htons (ETH_P_ARP);
	arp_pkt.hw_type = htons (0x0001);
	arp_pkt.hw_len = 6;
	arp_pkt.pro_type = htons (0x0800);
	arp_pkt.pro_len = 4;
	arp_pkt.arp_op = htons (ARPREQUEST);
	memcpy (arp_pkt.sender_eth,myMAC, 6);
	memcpy (arp_pkt.sender_ip, &myIP, 4);
	memcpy (arp_pkt.target_eth, "\xff\xff\xff\xff\xff\xff", 6);
	ip=kip;
	memcpy (arp_pkt.target_ip, &ip, 4);
	strcpy (from.sa_data,devname);
	from.sa_family = 1;
	len =sendto (rec, full_packet, sizeof (struct arp_struct), 0, &from, sizeof(from));
	if(0>=len){
			close(rec);return -1;
	}
	FD_ZERO(&fdset);
	FD_SET(rec, &fdset);

	retval = select(rec+1,&fdset,NULL,NULL,&tmout);
	if (0 >= retval){
			close(rec);return 1;
	}
	len = recvfrom (rec, full_packet, MAX_PACK_LEN, 0, &from, &from_len);
	memcpy (&ip, arp_pkt.target_ip, 4);
	memcpy (&sip, arp_pkt.sender_ip, 4);

	//have receive data suggest the IP has been used by some mac
	if(len>ETHER_HEADER_LEN){
		if(sip == inet_addr(ipaddr)) {
			close(rec);
			return 0;
		}
	}
	close(rec);
	return 1;
}
      
int main(int argc, char *argv[])
{
	char *ip = NULL;
	int i,j;
	int count= 0;
	struct in_addr ina;
	if(argc != 2)
	{
		fprintf(stderr,"bad argument\nipcheck [networkaddress]\n");
		exit(-1);
	}

	ip = argv[1];

	ina.s_addr = inet_addr(ip);
	ina.s_addr = ntohl(ina.s_addr);
	ina.s_addr &= 0xffffff00UL;
	ina.s_addr += 1;
	ina.s_addr = htonl(ina.s_addr);

	printf("starting from %s\n",inet_ntoa(ina));
	for(j=0;j < 254;j++)
	{
		ip = inet_ntoa(ina);
		i = do_ip_collision_probe(ip);
		if(1 == i)
		{
			//printf("IP: %s is not in use now.\n",ip);
		}
		else if(-1 == i) {
			printf("Error in arp process check if you have root privilege\n");
		}
		else
		{
			count ++;
			int i;
			printf("[%02d]IP: %s\tis in use! ",count,ip);
			printf("[MAC Address]: ");
			for(i=0;i < 5;i++) {
					printf("%02x:",arp_pkt.sender_eth[i]);
			}
			printf("%02x\n",arp_pkt.sender_eth[5]);
		}
		ina.s_addr = ntohl(ina.s_addr);
		ina.s_addr +=1;
		ina.s_addr = htonl(ina.s_addr);
		//binip++;
	}
	if(-i)
	{
		printf("-------------IP checking Statistics---------------\n");
		printf("Total %d IPs are in use in current network\n",count);
	}

	return 0;
}
