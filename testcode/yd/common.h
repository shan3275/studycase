#ifndef __COMMON_H__
#define __COMMON_H__

#include "init.h"

int pfring_parse_pkt(u_char *pkt, struct pfring_pkthdr *hdr, u_int8_t level /* L2..L4, 5 (tunnel) */,
        u_int8_t add_timestamp /* 0,1 */, u_int8_t add_hash /* 0,1 */);
#endif /* _PFRING_H_ */
