#ifndef __INIT_H__
#define __INIT_H__

#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sched.h>
#include <time.h>
#include <dirent.h>
#include <memory.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <signal.h>
#include <regex.h>
#include <signal.h>
#include <sched.h>
#include <sys/timeb.h>
#include <assert.h>
#include <ctype.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "zlog.h"
#include "pcap.h"
#include "zmq.h"
#include "event.h"

#define MAX_IP_LEN 32
#define MAX_HOST_IDFA_LEN 40960
#define MAX_APPID_NAME_LEN 1024
#define __GET_DETAIL_INFO__


#define NO_TUNNEL_ID     0xFFFFFFFF

typedef union {
    struct in6_addr v6;  /* IPv6 src/dst IP addresses (Network byte order) */
    u_int32_t v4;        /* IPv4 src/dst IP addresses */
} ip_addr;

typedef struct {
    u_int32_t tunnel_id; /* GTP/GRE tunnelId or NO_TUNNEL_ID for no filtering */
    u_int8_t tunneled_proto;
    ip_addr tunneled_ip_src, tunneled_ip_dst;
    u_int16_t tunneled_l4_src_port, tunneled_l4_dst_port;
} tunnel_info;

typedef struct pkt_offset {
    int16_t eth_offset; /*
                         This offset *must* be added to all offsets below
                         ONLY if you are inside the kernel (e.g. when you
                         code a pf_ring plugin). Ignore it in user-space.
                       */
    int16_t vlan_offset;
    int16_t l3_offset;
    int16_t l4_offset;
    int16_t payload_offset;
}pkt_offset;

#define GRE_HEADER_CHECKSUM     0x8000
#define GRE_HEADER_ROUTING      0x4000
#define GRE_HEADER_KEY          0x2000
#define GRE_HEADER_SEQ_NUM      0x1000
#define GRE_HEADER_VERSION      0x0007

struct gre_header {
    u_int16_t flags_and_version;
    u_int16_t proto;
    /* Optional fields */
};

#ifndef ETH_ALEN
#define ETH_ALEN  6
#endif
struct pkt_parsing_info {
    /* Core fields (also used by NetFlow) */
    u_int8_t dmac[ETH_ALEN], smac[ETH_ALEN];  /* MAC src/dst addresses */
    u_int16_t eth_type;   /* Ethernet type */
    u_int16_t vlan_id;    /* VLAN Id or NO_VLAN */
    u_int8_t  ip_version;
    u_int8_t  l3_proto, ip_tos; /* Layer 3 protocol/TOS */
    ip_addr   ip_src, ip_dst;   /* IPv4 src/dst IP addresses */
    u_int16_t l4_src_port, l4_dst_port; /* Layer 4 src/dst ports */
    struct {
        u_int8_t flags;   /* TCP flags (0 if not available) */
        u_int32_t seq_num, ack_num; /* TCP sequence number */
    } tcp;

    tunnel_info tunnel;
    u_int16_t last_matched_plugin_id; /* If > 0 identifies a plugin to that matched the packet */
    u_int16_t last_matched_rule_id; /* If > 0 identifies a rule that matched the packet */
    struct pkt_offset offset; /* Offsets of L3/L4/payload elements */
};


struct pfring_extended_pkthdr {
    u_int64_t timestamp_ns;  /* Packet timestamp at ns precision. Note that if your NIC supports
                              hardware timestamp, this is the place to read timestamp from */
    u_int8_t rx_direction;   /* 1=RX: packet received by the NIC, 0=TX: packet transmitted by the NIC */
    int32_t  if_index;       /* index of the interface on which the packet has been received.
                              It can be also used to report other information */
    u_int32_t pkt_hash;      /* Hash based on the packet header */
    struct {
        int bounce_interface; /* Interface Id where this packet will bounce after processing
                             if its values is other than UNKNOWN_INTERFACE */
        struct sk_buff *reserved; /* Kernel only pointer */
    } tx;
    u_int16_t parsed_header_len; /* Extra parsing data before packet */

    /* NOTE: leave it as last field of the memset on parse_pkt() will fail */
    struct pkt_parsing_info parsed_pkt; /* packet parsing info */
};


struct pfring_pkthdr
{
    /* pcap header */
    struct timeval ts;    /* time stamp */
    u_int32_t caplen;     /* length of portion present */
    u_int32_t len;        /* length this packet (off wire) */
    struct pfring_extended_pkthdr extended_hdr; /* PF_RING extended header */
};

#define ipv4_tos     ip_tos
#define ipv6_tos     ip_tos
#define ipv4_src     ip_src.v4
#define ipv4_dst     ip_dst.v4
#define ipv6_src     ip_src.v6
#define ipv6_dst     ip_dst.v6
#define host4_low    host_low.v4
#define host4_high   host_high.v4
#define host6_low    host_low.v6
#define host6_high   host_high.v6
#define host4_peer_a host_peer_a.v4
#define host4_peer_b host_peer_b.v4
#define host6_peer_a host_peer_a.v6
#define host6_peer_b host_peer_b.v6


#define __LITTLE_ENDIAN_BITFIELD /* FIX */
struct ethhdr {
    unsigned char   h_dest[ETH_ALEN];       /* destination eth addr */
    unsigned char   h_source[ETH_ALEN];     /* source ether addr    */
    u_int16_t       h_proto;                /* packet type ID field */
};

/* defined in pf_ring.h */
struct eth_vlan_hdr {
    u_int16_t       h_vlan_id;              /* Tag Control Information (QoS, VLAN ID) */
    u_int16_t       h_proto;                /* packet type ID field */
};

struct iphdr {
#if defined(__LITTLE_ENDIAN_BITFIELD)
    u_int8_t    ihl:4,
            version:4;
#elif defined (__BIG_ENDIAN_BITFIELD)
  u_int8_t      version:4,
    ihl:4;
#else
#error  "Please fix <asm/byteorder.h>"
#endif
    u_int8_t    tos;
    u_int16_t   tot_len;
    u_int16_t   id;
#define IP_CE           0x8000
#define IP_DF           0x4000
#define IP_MF           0x2000
#define IP_OFFSET       0x1FFF
    u_int16_t   frag_off;
    u_int8_t    ttl;
    u_int8_t    protocol;
    u_int16_t   check;
    u_int32_t   saddr;
    u_int32_t   daddr;
    /*The options start here. */
};

struct tcphdr {
      u_int16_t       source;
      u_int16_t       dest;
      u_int32_t       seq;
      u_int32_t       ack_seq;
#if defined(__LITTLE_ENDIAN_BITFIELD)
      u_int16_t       res1:4,
doff:4,
fin:1,
syn:1,
rst:1,
psh:1,
ack:1,
urg:1,
ece:1,
cwr:1;
#elif defined(__BIG_ENDIAN_BITFIELD)
      u_int16_t       doff:4,
res1:4,
cwr:1,
ece:1,
urg:1,
ack:1,
psh:1,
rst:1,
syn:1,
fin:1;
#else
#error        "Adjust your <asm/byteorder.h> defines"
#endif
      u_int16_t       window;
     u_int16_t       check;
      u_int16_t       urg_ptr;
};



struct udphdr {
    u_int16_t   source;
    u_int16_t   dest;
    u_int16_t   len;
    u_int16_t   check;
};

#define TH_FIN_MULTIPLIER       0x01
#define TH_SYN_MULTIPLIER       0x02
#define TH_RST_MULTIPLIER       0x04
#define TH_PUSH_MULTIPLIER      0x08
#define TH_ACK_MULTIPLIER       0x10
#define TH_URG_MULTIPLIER       0x20

struct ipv6hdr {
#if defined(__LITTLE_ENDIAN_BITFIELD)
    u_int8_t            priority:4,
            version:4;
#elif defined(__BIG_ENDIAN_BITFIELD)
  u_int8_t              version:4,
                priority:4;
#else
#endif
    u_int8_t            flow_lbl[3];

    u_int8_t            payload_len;
    u_int8_t            nexthdr;
    u_int8_t            hop_limit;

    struct in6_addr saddr;
    struct in6_addr daddr;
};

struct ipv6_opt_hdr {
    u_int8_t            nexthdr;
    u_int8_t            hdrlen;
    /* TLV encoded option data follows */
} __attribute__((packed));

#define NEXTHDR_HOP               0
#define NEXTHDR_TCP               6
#define NEXTHDR_UDP              17
#define NEXTHDR_IPV6             41
#define NEXTHDR_ROUTING          43
#define NEXTHDR_FRAGMENT         44
#define NEXTHDR_ESP              50
#define NEXTHDR_AUTH             51
#define NEXTHDR_ICMP             58
#define NEXTHDR_NONE             59
#define NEXTHDR_DEST             60
#define NEXTHDR_MOBILITY        135

/////////////////////////////////////////////////////////////////
/* http parser used structure */
struct http_request_kinfo
{
    u_char *uri_start;
    u_char *uri_end;
    u_char *host_start;
    u_char *host_end;
    u_char *ua_start;
    u_char *ua_end;
    u_char *referer_start;
    u_char *referer_end;
    u_char *cookies_start;
    u_char *cookies_end;
    u_char *content_len_start;
    u_char *content_len_end;
    u_char *post_start;
    u_char *post_end;
    int http_type;
    u_int sip;
    u_int dip;
    time_t sec;
    time_t usec;
    char timestamp[25];
    u_char *content;        /* tcp payload */
    u_int len;              /* tcp payload length */
    int header_state;       /* record context */
};

struct compact_eth_hdr
{
    unsigned char   h_dest[6];
    unsigned char   h_source[6];
    u_int16_t       h_proto;
};

struct compact_ip_hdr
{
    u_int8_t    ihl:4,
            version:4;
    u_int8_t    tos;
    u_int16_t   tot_len;
    u_int16_t   id;
    u_int16_t   frag_off;
    u_int8_t    ttl;
    u_int8_t    protocol;
    u_int16_t   check;
    u_int32_t   saddr;
    u_int32_t   daddr;
};



#endif
