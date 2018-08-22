#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <linux/in.h>
#include "head.h"
 
 
//caculate ip checksum
unsigned short int ip_csum (unsigned short int *addr, int len)
{
	int nleft = len;
        int sum = 0;
        unsigned short int *w = addr;
	unsigned short int answer = 0; 
        while (nleft > 1) 
	{
		sum += *w++;
		nleft -= sizeof (unsigned short int);
	}
		 
	if (nleft == 1) 
	{
	 	*(char *) (&answer) = *(char *) w;
		sum += answer;
	}
	
	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	answer = ~sum;
	return (answer);
}
 
#if 1 
//caculate udp checksum (include payload)
unsigned short int udp_csum (struct ipheader iphdr, struct udpheader udphdr, unsigned char *payload, int payloadlen)
{
  char buf[65535];
  char *ptr;
  int chksumlen = 0;
  int i;
 
  ptr = &buf[0];  // ptr points to beginning of buffer buf
 
  /****************pseudo header  12byte*********************************/
 
  // Copy source IP address into buf (32 bits)
  memcpy (ptr, &iphdr.iph_sourceip, sizeof (iphdr.iph_sourceip));
  ptr += sizeof (iphdr.iph_sourceip);
  chksumlen += sizeof (iphdr.iph_sourceip);
 
  // Copy destination IP address into buf (32 bits)
  memcpy (ptr, &iphdr.iph_destip, sizeof (iphdr.iph_destip));
  ptr += sizeof (iphdr.iph_destip);
  chksumlen += sizeof (iphdr.iph_destip);
 
  // Copy zero field to buf (8 bits)
  *ptr = 0; ptr++;
  chksumlen += 1;
 
  // Copy transport layer protocol to buf (8 bits)
  memcpy (ptr, &iphdr.iph_protocol, sizeof (iphdr.iph_protocol));
  ptr += sizeof (iphdr.iph_protocol);
  chksumlen += sizeof (iphdr.iph_protocol);
 
  // Copy UDP length to buf (16 bits)
  memcpy (ptr, &udphdr.udph_len, sizeof (udphdr.udph_len));
  ptr += sizeof (udphdr.udph_len);
  chksumlen += sizeof (udphdr.udph_len);
 
  /*******************end pseudo header*************************/
  
  // Copy UDP source port to buf (16 bits)
  memcpy (ptr, &udphdr.udph_srcport, sizeof (udphdr.udph_srcport));
  ptr += sizeof (udphdr.udph_srcport);
  chksumlen += sizeof (udphdr.udph_srcport);
 
  // Copy UDP destination port to buf (16 bits)
  memcpy (ptr, &udphdr.udph_destport, sizeof (udphdr.udph_destport));
  ptr += sizeof (udphdr.udph_destport);
  chksumlen += sizeof (udphdr.udph_destport);
 
  // Copy UDP length again to buf (16 bits)
  memcpy (ptr, &udphdr.udph_len, sizeof (udphdr.udph_len));
  ptr += sizeof (udphdr.udph_len);
  chksumlen += sizeof (udphdr.udph_len);
 
  // Copy UDP checksum to buf (16 bits)
  // Zero, since we don't know it yet
  *ptr = 0; ptr++;
  *ptr = 0; ptr++;
  chksumlen += 2;
 
  // Copy payload to buf
  memcpy (ptr, payload, payloadlen);
  ptr += payloadlen;
  chksumlen += payloadlen;
 
  // Pad to the next 16-bit boundary
  for (i=0; i<payloadlen%2; i++, ptr++) {
    *ptr = 0;
    ptr++;
    chksumlen++;
  }
 
  return ip_csum ((unsigned short int *) buf, chksumlen);
}
#endif
 
 
int main(int argc, char *argv[])
{
	struct sockaddr_ll device;
	int sd;
	char buffer[1024] = {0};
 
	struct ether *eth = (struct ether *)malloc(sizeof(struct ether));
	struct ipheader *ip = (struct ipheader *)malloc(sizeof(struct ipheader));
	struct udpheader *udp = (struct udpheader *)malloc(sizeof(struct udpheader));
	//struct tcphdr   *tcph = (struct tcphdr *)malloc(sizeof(struct tcphdr)); 
 
	if(argc != 5)
	{
		printf("- Invalid parameters!!!\n");
		printf("- Usage %s<source hostname/IP> <source port> <target hostname/IP> <target port>\n", argv[0]);
		exit(-1);
 
	}
	
	/*****************fill udp header*********************************/
	//udp payload
	char data[9] = {'1','2','3','4','5','6','7','8','9'};
	
	//udp header
	udp->udph_srcport = htons(atoi(argv[2]));
 
	udp->udph_destport = htons(atoi(argv[4]));
 
	udp->udph_len = htons(sizeof(struct udpheader)+sizeof(data));
 
	udp->udph_chksum =udp_csum(*ip,*udp,data,sizeof(data));//options
 
	/*******************end fill udp header**************************/
	
#if 0
	/********************fill tcp header*************************/
	tcph->source = htons(6666);
	tcph->dest = htons(6666);
	tcph->seq = random();
	tcph->ack_seq = 0;
	tcph->tcp_len = 0x0250;
	#if 0
	tcph->res1 = 0;
	tcph->res2 = 0;
	tcph->urg = 0;
	tcph->ack = 0;
	tcph->psh = 0;
	tcph->rst = 0;
	tcph->syn = 0;
	tcph->fin = 0;
	#endif
	tcph->window = htons(20480);
	tcph->check = 0;//caculate chksum and fill it
	tcph->urg_prt = 0;
	/*****************end fill tcp header**************************/
#endif	
	/********************fill ip header****************************/
	//fill ip header (20 bytes)
	
	ip->iplv = 1<<6 | 5;//version v4 or v6 and head_len 5 
			   //ip首部长度以32bit为单位计算
 
	ip->iph_tos = 0; // Low delay
 
	ip->iph_len = htons(sizeof(struct ipheader) + sizeof(struct udpheader) +sizeof(data));
 
	ip->iph_ident = 0;//标示字段 唯一标示一个数据包
 
	ip->iph_offset = 0x0040;//16bit include offset and flag
 
	ip->iph_ttl = 64; // time to live
 
	ip->iph_protocol = 17; // UDP
 
	// Calculate the checksum for integrity
	ip->iph_chksum = ip_csum((unsigned short int *)ip, (int)sizeof(struct ipheader));
	
	// Source IP address, can use spoofed address here!!!
 
	ip->iph_sourceip = inet_addr(argv[1]);
 
	// The destination IP address
		
	ip->iph_destip = inet_addr(argv[3]);
 
	/******************end fill ip header**********************/
	
	/********************fill eth header*********************/
	//  Fill the eth header (14bytes)
	eth->smac[0] = 0x88;
	eth->smac[1] = 0x78;
	eth->smac[2] = 0x56;
	eth->smac[3] = 0x8c;
	eth->smac[4] = 0x58;
	eth->smac[5] = 0x0c;
 
	eth->dmac[0] = 0xFF;
	eth->dmac[1] = 0xFF;
	eth->dmac[2] = 0xFF;
	eth->dmac[3] = 0xFF;
	eth->dmac[4] = 0xFF;
	eth->dmac[5] = 0xFF;
 
	eth->eth_typ_len[0] = 0x08;
	eth->eth_typ_len[1] = 0x00;
	/***********************end fill eth header*****************/
 
	memcpy(buffer,eth,sizeof(struct ether));
	memcpy(buffer + sizeof(struct ether),ip,sizeof(struct ipheader));
	memcpy(buffer + sizeof(struct ether) + sizeof(struct ipheader),udp,sizeof(struct udpheader) );
	memcpy(buffer + sizeof(struct ether) + sizeof(struct ipheader) + sizeof(struct udpheader),data,sizeof(data));
 
	/*************************fill sockaddr_ll***********************/
        /*
        利用原始套接字发送数据，那么发送的时候需要自己组织整个以太网数据帧.所有相关的地址使用struct sockaddr_ll 而不是struct sockaddr_in(因为协议簇是PF_PACKET不是AF_INET了),比如发送给某个机器,对方的地址需要使用struct sockaddr_ll.
        */
	memset(&device,0,sizeof(device));
	if((device.sll_ifindex = if_nametoindex("eth0")) == 0)
	{
		printf("if_nametoindex() failed to obtain interface index\n");
		exit(EXIT_FAILURE);
	}
	printf("index of interface eth0 is %i\n",device.sll_ifindex);
	device.sll_family = AF_PACKET;
	memcpy(device.sll_addr,eth->dmac,6);
	device.sll_halen = htons(6);
	/******************end fill sockaddr_ll***************************/
 
	// Create a raw socket with UDP protocol
	if((sd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0)
	{
		printf("create SOCK_RAW socket error\n");
		exit(-1);
	}
	else
	{
		printf("create SOCK_RAW socket OK.\n");
	}
 
	while(1)
	{
		//send udp packet
		if(sendto(sd, (void *)buffer,sizeof(struct ether)+sizeof(struct ipheader)+sizeof(struct udpheader)+sizeof(data), 0, (struct sockaddr *)&device, sizeof(device)) < 0)
		
		{
			perror("sendto() error");
			exit(-1);
		}
		else
		{
			printf("sendto() is OK.\n");
			sleep(1);
		}
 
	}
	close(sd);
	return 0;
}
