#include "common.h"

int pfring_parse_pkt(u_char *pkt, struct pfring_pkthdr *hdr, u_int8_t level /* L2..L4, 5 (tunnel) */,
        u_int8_t add_timestamp /* 0,1 */, u_int8_t add_hash /* 0,1 */) {
    struct ethhdr *eh = (struct ethhdr*) pkt;
    u_int32_t displ = 0, ip_len;
    u_int16_t analyzed = 0, fragment_offset = 0;

    hdr->extended_hdr.parsed_pkt.tunnel.tunnel_id = -1;

    /* Note: in order to optimize the computation, this function expects a zero-ed
     * or partially parsed pkthdr */
    //memset(&hdr->extended_hdr.parsed_pkt, 0, sizeof(struct pkt_parsing_info));
    //hdr->extended_hdr.parsed_header_len = 0;

    hdr->extended_hdr.parsed_pkt.offset.l3_offset = 0;
    hdr->extended_hdr.parsed_pkt.offset.l4_offset = 0;

    if (hdr->extended_hdr.parsed_pkt.offset.l3_offset != 0)
        goto L3;

    memcpy(&hdr->extended_hdr.parsed_pkt.dmac, eh->h_dest,   sizeof(eh->h_dest));
    memcpy(&hdr->extended_hdr.parsed_pkt.smac, eh->h_source, sizeof(eh->h_source));

    hdr->extended_hdr.parsed_pkt.eth_type = ntohs(eh->h_proto);
    hdr->extended_hdr.parsed_pkt.offset.eth_offset = 0;
    hdr->extended_hdr.parsed_pkt.offset.vlan_offset = 0;
    hdr->extended_hdr.parsed_pkt.vlan_id = 0; /* Any VLAN */

    if (hdr->extended_hdr.parsed_pkt.eth_type == 0x8100 /* 802.1q (VLAN) */) {
        struct eth_vlan_hdr *vh;
        hdr->extended_hdr.parsed_pkt.offset.vlan_offset = sizeof(struct ethhdr) - sizeof(struct eth_vlan_hdr);
        while (hdr->extended_hdr.parsed_pkt.eth_type == 0x8100 /* 802.1q (VLAN) */ ) {
            hdr->extended_hdr.parsed_pkt.offset.vlan_offset += sizeof(struct eth_vlan_hdr);
            vh = (struct eth_vlan_hdr *) &pkt[hdr->extended_hdr.parsed_pkt.offset.vlan_offset];
            hdr->extended_hdr.parsed_pkt.vlan_id = ntohs(vh->h_vlan_id) & 0x0fff;
            hdr->extended_hdr.parsed_pkt.eth_type = ntohs(vh->h_proto);
            displ += sizeof(struct eth_vlan_hdr);
        }
    }

    hdr->extended_hdr.parsed_pkt.offset.l3_offset = hdr->extended_hdr.parsed_pkt.offset.eth_offset + displ + sizeof(struct ethhdr);

    L3:

    analyzed = 2;

    if (level < 3)
        goto TIMESTAMP;

    if (hdr->extended_hdr.parsed_pkt.offset.l4_offset != 0)
        goto L4;

    if (hdr->extended_hdr.parsed_pkt.eth_type == 0x0800 /* IPv4 */) {
        struct iphdr *ip;

        hdr->extended_hdr.parsed_pkt.ip_version = 4;

        if (hdr->caplen < hdr->extended_hdr.parsed_pkt.offset.l3_offset + sizeof(struct iphdr))
            goto TIMESTAMP;

        ip = (struct iphdr *)(&pkt[hdr->extended_hdr.parsed_pkt.offset.l3_offset]);

        hdr->extended_hdr.parsed_pkt.ipv4_src = ntohl(ip->saddr);
        hdr->extended_hdr.parsed_pkt.ipv4_dst = ntohl(ip->daddr);
        hdr->extended_hdr.parsed_pkt.l3_proto = ip->protocol;
        hdr->extended_hdr.parsed_pkt.ipv4_tos = ip->tos;
        fragment_offset = ip->frag_off & htons(IP_OFFSET); /* fragment, but not the first */
        ip_len  = ip->ihl*4;

    } else if(hdr->extended_hdr.parsed_pkt.eth_type == 0x86DD /* IPv6 */) {
        struct ipv6hdr *ipv6;

        hdr->extended_hdr.parsed_pkt.ip_version = 6;

        if(hdr->caplen < hdr->extended_hdr.parsed_pkt.offset.l3_offset + sizeof(struct ipv6hdr))
            goto TIMESTAMP;

        ipv6 = (struct ipv6hdr*)(&pkt[hdr->extended_hdr.parsed_pkt.offset.l3_offset]);
        ip_len = sizeof(struct ipv6hdr);

        /* Values of IPv6 addresses are stored as network byte order */
        //memcpy(&hdr->extended_hdr.parsed_pkt.ipv6_src, &ipv6->saddr, sizeof(ipv6->saddr));
        ///memcpy(&hdr->extended_hdr.parsed_pkt.ipv6_dst, &ipv6->daddr, sizeof(ipv6->daddr));

        hdr->extended_hdr.parsed_pkt.l3_proto = ipv6->nexthdr;
        hdr->extended_hdr.parsed_pkt.ipv6_tos = ipv6->priority; /* IPv6 class of service */

        while(hdr->extended_hdr.parsed_pkt.l3_proto == NEXTHDR_HOP	   ||
                hdr->extended_hdr.parsed_pkt.l3_proto == NEXTHDR_DEST	   ||
                hdr->extended_hdr.parsed_pkt.l3_proto == NEXTHDR_ROUTING ||
                hdr->extended_hdr.parsed_pkt.l3_proto == NEXTHDR_AUTH	   ||
                hdr->extended_hdr.parsed_pkt.l3_proto == NEXTHDR_ESP	   ||
                hdr->extended_hdr.parsed_pkt.l3_proto == NEXTHDR_FRAGMENT) {
            struct ipv6_opt_hdr *ipv6_opt;

            if(hdr->caplen < hdr->extended_hdr.parsed_pkt.offset.l3_offset + ip_len + sizeof(struct ipv6_opt_hdr))
                goto TIMESTAMP;

            ipv6_opt = (struct ipv6_opt_hdr *)(&pkt[hdr->extended_hdr.parsed_pkt.offset.l3_offset + ip_len]);
            ip_len += sizeof(struct ipv6_opt_hdr);
            if(hdr->extended_hdr.parsed_pkt.l3_proto == NEXTHDR_AUTH)
                    /*
                      RFC4302 2.2. Payload Length: This 8-bit field specifies the
                      length of AH in 32-bit words (4-byte units), minus "2".
                    */
                ip_len += ipv6_opt->hdrlen * 4;
            else if(hdr->extended_hdr.parsed_pkt.l3_proto != NEXTHDR_FRAGMENT)
                ip_len += ipv6_opt->hdrlen;

            hdr->extended_hdr.parsed_pkt.l3_proto = ipv6_opt->nexthdr;
        }
    } else {
        hdr->extended_hdr.parsed_pkt.l3_proto = 0;
        goto TIMESTAMP;
    }

    hdr->extended_hdr.parsed_pkt.offset.l4_offset = hdr->extended_hdr.parsed_pkt.offset.l3_offset + ip_len;

    L4:

    analyzed = 3;

    if (level < 4 || fragment_offset)
        goto TIMESTAMP;

    if(hdr->extended_hdr.parsed_pkt.l3_proto == IPPROTO_TCP) {
        struct tcphdr *tcp;

        if(hdr->caplen < hdr->extended_hdr.parsed_pkt.offset.l4_offset + sizeof(struct tcphdr))
            goto TIMESTAMP;

        tcp = (struct tcphdr *)(&pkt[hdr->extended_hdr.parsed_pkt.offset.l4_offset]);

        hdr->extended_hdr.parsed_pkt.l4_src_port = ntohs(tcp->source);
        hdr->extended_hdr.parsed_pkt.l4_dst_port = ntohs(tcp->dest);
        hdr->extended_hdr.parsed_pkt.offset.payload_offset = hdr->extended_hdr.parsed_pkt.offset.l4_offset + (tcp->doff * 4);
        hdr->extended_hdr.parsed_pkt.tcp.seq_num = ntohl(tcp->seq);
        hdr->extended_hdr.parsed_pkt.tcp.ack_num = ntohl(tcp->ack_seq);
        hdr->extended_hdr.parsed_pkt.tcp.flags = (tcp->fin * TH_FIN_MULTIPLIER) + (tcp->syn * TH_SYN_MULTIPLIER) +
                (tcp->rst * TH_RST_MULTIPLIER) + (tcp->psh * TH_PUSH_MULTIPLIER) +
                (tcp->ack * TH_ACK_MULTIPLIER) + (tcp->urg * TH_URG_MULTIPLIER);

        analyzed = 4;
    } else if(hdr->extended_hdr.parsed_pkt.l3_proto == IPPROTO_UDP) {
        struct udphdr *udp;

        if(hdr->caplen < hdr->extended_hdr.parsed_pkt.offset.l4_offset + sizeof(struct udphdr))
            goto TIMESTAMP;

        udp = (struct udphdr *)(&pkt[hdr->extended_hdr.parsed_pkt.offset.l4_offset]);

        hdr->extended_hdr.parsed_pkt.l4_src_port = ntohs(udp->source), hdr->extended_hdr.parsed_pkt.l4_dst_port = ntohs(udp->dest);
        hdr->extended_hdr.parsed_pkt.offset.payload_offset = hdr->extended_hdr.parsed_pkt.offset.l4_offset + sizeof(struct udphdr);

        analyzed = 4;

        if (level < 5)
            goto TIMESTAMP;

    } else if(hdr->extended_hdr.parsed_pkt.l3_proto == IPPROTO_GRE /* 0x47 */) {
        struct gre_header *gre = (struct gre_header*)(&pkt[hdr->extended_hdr.parsed_pkt.offset.l4_offset]);
        int gre_offset;

        gre->flags_and_version = ntohs(gre->flags_and_version);
        gre->proto = ntohs(gre->proto);

        gre_offset = sizeof(struct gre_header);

        if((gre->flags_and_version & GRE_HEADER_VERSION) == 0) {
            if(gre->flags_and_version & (GRE_HEADER_CHECKSUM | GRE_HEADER_ROUTING)) gre_offset += 4;
            if(gre->flags_and_version & GRE_HEADER_KEY) {
                u_int32_t *tunnel_id = (u_int32_t*)(&pkt[hdr->extended_hdr.parsed_pkt.offset.l4_offset+gre_offset]);
                gre_offset += 4;
                hdr->extended_hdr.parsed_pkt.tunnel.tunnel_id = ntohl(*tunnel_id);
            }
            if(gre->flags_and_version & GRE_HEADER_SEQ_NUM)  gre_offset += 4;

            hdr->extended_hdr.parsed_pkt.offset.payload_offset = hdr->extended_hdr.parsed_pkt.offset.l4_offset + gre_offset;

            analyzed = 4;

            if (level < 5)
                goto TIMESTAMP;


        } else { /* TODO handle other GRE versions */
            hdr->extended_hdr.parsed_pkt.offset.payload_offset = hdr->extended_hdr.parsed_pkt.offset.l4_offset;
        }
    } else {
        hdr->extended_hdr.parsed_pkt.offset.payload_offset = hdr->extended_hdr.parsed_pkt.offset.l4_offset;
        hdr->extended_hdr.parsed_pkt.l4_src_port = hdr->extended_hdr.parsed_pkt.l4_dst_port = 0;
    }

    TIMESTAMP:

    gettimeofday(&hdr->ts, NULL); /* TODO What about using clock_gettime(CLOCK_REALTIME, ts) ? */

    return analyzed;
}