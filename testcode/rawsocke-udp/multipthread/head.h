#ifndef _HEAD_H
#define _HEAD_H    
 
#define BUFFER_MAX 2048
#define PCKT_LEN 8492
 
#pragma pack (1)
//ethernet head 14 bytes
struct ether
{
	char dmac[6];
	char smac[6];
	char eth_typ_len[2];
};
 
 
//ip head 20 bytes
struct ipheader {
 
 unsigned char      iplv;//iph_ihl:5, iph_ver:4;
 
 unsigned char      iph_tos;
 
 unsigned short int iph_len;
 
 unsigned short int iph_ident;
 
// unsigned uint8_t      iph_flag;
 
 unsigned short int iph_offset;//16bit include flag and offset
 
 unsigned char      iph_ttl;
 
 unsigned char      iph_protocol;
 
 unsigned short int iph_chksum;
 
 unsigned int       iph_sourceip;
 
 unsigned int       iph_destip;
 
};
 
//udp head 8 bytes
struct udpheader {
 
 unsigned short int udph_srcport;
 
 unsigned short int udph_destport;
 
 unsigned short int udph_len;
 
 unsigned short int udph_chksum;
 
};
 
//tcp head 20bytes
struct tcphdr
{
    unsigned short int source;  
    unsigned short int dest;   
    unsigned int seq;  
    unsigned int ack_seq; 
    unsigned short int tcp_len;
#if 0
    unsigned short int doff:4;
    unsigned short int res1:4;
    unsigned short int res2:2;
    unsigned short int urg:1;
    unsigned short int ack:1;
    unsigned short int psh:1;
    unsigned short int rst:1;
    unsigned short int syn:1;
    unsigned short int fin:1;
#endif
    unsigned short int window;  
    unsigned short int check;  
    unsigned short int urg_prt;  
};
 
#pragma pack ()
 
#endif
