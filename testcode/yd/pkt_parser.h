#ifndef __PKTPARSE_H__
#define __PKTPARSE_H__

#include "init.h"

#define SNAP_LEN        65535    /*default snap length (maximum bytes per packet to capture) */
#define SIZE_ETHERNET   14       /*ethernet headers are always exactly 14 bytes [1]*/
#define SIZE_GRE        8
#define ETHER_ADDR_LEN	6        /*Ethernet addresses are 6 bytes*/
#define CR (u_char)     13       /* CR LF define */
#define LF (u_char)     10       /* CR LF define */


/* Ethernet header */
struct sniff_ethernet
{
	u_char ether_dhost[ETHER_ADDR_LEN]; /* destination host address */
	u_char ether_shost[ETHER_ADDR_LEN]; /* source host address */
	u_short ether_type; /* IP? ARP? RARP? etc */
};
/* IPv4 header */
struct sniff_ip
{
	u_char ip_vhl; /* version << 4 | header length >> 2 */
	u_char ip_tos; /* type of service */
	u_short ip_len; /* total length */
	u_short ip_id; /* identification */
	u_short ip_off; /* fragment offset field */
	u_char ip_ttl; /* time to live */
	u_char ip_p; /* protocol */
	u_short ip_sum; /* checksum */
	struct in_addr ip_src, ip_dst; /* source and dest address */
};

#define IP_RF 0x8000            /* reserved fragment flag */
#define IP_DF 0x4000            /* dont fragment flag */
#define IP_MF 0x2000            /* more fragments flag */
#define IP_OFFMASK 0x1fff       /* mask for fragmenting bits */
#define IP_HL(ip)               (((ip)->ip_vhl) & 0x0f)
#define IP_V(ip)                (((ip)->ip_vhl) >> 4)

/* TCP header */
typedef u_int tcp_seq;

struct sniff_tcp
{
	u_short th_sport; /* source port */
	u_short th_dport; /* destination port */
	tcp_seq th_seq; /* sequence number */
	tcp_seq th_ack; /* acknowledgement number */
	u_char th_offx2; /* data offset, rsvd */
	u_char th_flags;
	u_short th_win; /* window */
	u_short th_sum; /* checksum */
	u_short th_urp; /* urgent pointer */
};
#define TH_OFF(th)      (((th)->th_offx2 & 0xf0) >> 4)
#define TH_FIN  0x01
#define TH_SYN  0x02
#define TH_RST  0x04
#define TH_PUSH 0x08
#define TH_ACK  0x10
#define TH_URG  0x20
#define TH_ECE  0x40
#define TH_CWR  0x80
#define TH_FLAGS  (TH_FIN|TH_SYN|TH_RST|TH_ACK|TH_URG|TH_ECE|TH_CWR)

/* define http header segment state , the state just recode context infomation */
#define HS_HOST         1
#define HS_USER_AGENT   2
#define HS_REFERER      3
#define HS_COOKIE       4
#define HS_CONTENT_LENGTH   5

#define str3_cmp(m, c0, c1, c2, c3)                                       \
    *(uint32_t *) m == ((c3 << 24) | (c2 << 16) | (c1 << 8) | c0)

#define str3Ocmp(m, c0, c1, c2, c3)                                       \
    *(uint32_t *) m == ((c3 << 24) | (c2 << 16) | (c1 << 8) | c0)

#define str4cmp(m, c0, c1, c2, c3)                                        \
    *(uint32_t *) m == ((c3 << 24) | (c2 << 16) | (c1 << 8) | c0)

#define str5cmp(m, c0, c1, c2, c3, c4)                                    \
    *(uint32_t *) m == ((c3 << 24) | (c2 << 16) | (c1 << 8) | c0)             \
		        && m[4] == c4

#define str6cmp(m, c0, c1, c2, c3, c4, c5)                                \
    *(uint32_t *) m == ((c3 << 24) | (c2 << 16) | (c1 << 8) | c0)             \
	&& (((uint32_t *) m)[1] & 0xffff) == ((c5 << 8) | c4)

#define str7cmp(m, c0, c1, c2, c3, c4, c5, c6, c7)                       \
    *(uint32_t *) m == ((c3 << 24) | (c2 << 16) | (c1 << 8) | c0)             \
        && ((uint32_t *) m)[1] == ((c7 << 24) | (c6 << 16) | (c5 << 8) | c4)

#define str8cmp(m, c0, c1, c2, c3, c4, c5, c6, c7)                        \
    *(uint32_t *) m == ((c3 << 24) | (c2 << 16) | (c1 << 8) | c0)             \
        && ((uint32_t *) m)[1] == ((c7 << 24) | (c6 << 16) | (c5 << 8) | c4)

#define str9cmp(m, c0, c1, c2, c3, c4, c5, c6, c7, c8)                    \
    *(uint32_t *) m == ((c3 << 24) | (c2 << 16) | (c1 << 8) | c0)             \
        && ((uint32_t *) m)[1] == ((c7 << 24) | (c6 << 16) | (c5 << 8) | c4)  \
	&& m[8] == c8

/*
 * transfer int to str, should be very fast
 */
void http_parser_int2str(u_int num, char **str);
void http_parser_int2str_with_wd(u_int num, char **str, int width);
int http_parser_atoi(const char * start, const char * end);
/*
 * append str to another str, should be very fast
 */
void http_parser_str_append(char ** dst, const char * str);

 /* transfer eth packet to http packet
 */
int http_parser_level_1(int thread_id, struct pfring_pkthdr * hdr, u_char *buffer, struct http_request_kinfo *http);
/*
 * parser eth packet buffer to struct
 */
int http_parser_level_2(int thread_id,struct pfring_pkthdr * hdr, u_char *buffer, struct http_request_kinfo *httprequest);

/*
 * transfer http struct to json string
 */
int http_parser_level_3(struct http_request_kinfo *httprequest, char *str, struct pfring_pkthdr *hdr);

/*
 * print data in rows of 16 bytes: offset   hex   ascii
 *
 * 00000   47 45 54 20 2f 20 48 54  54 50 2f 31 2e 31 0d 0a   GET / HTTP/1.1..
 */
void http_parser_print_hex_ascii_line(const u_char *payload, int len, int offset);

/*
 * print packet payload data (avoid printing binary data)
 */
void http_parser_print_payload(const u_char *payload, int len);

void print(u_char* ucParam, int nlen);

#endif
