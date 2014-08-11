#include "md_config.h"
#ifndef __MD_STS_INCLUDE__
#define __MD_STS_INCLUDE__

typedef struct md_pkt_sts_t
{
    uint32_t get;
    uint32_t fwd;
    uint32_t drop;
    uint32_t permit;
    uint32_t free;
}md_pkt_sts;
int md_sts_init(void);
void md_sts_get_add(void);
void md_sts_free_add(void);
void md_sts_fwd_add(void);
void md_sts_drop_add(void);
void md_sts_permit_add(void);
int md_sts_clear(void );
int  md_sts_get(md_pkt_sts *sts);

#endif /* end of __MD_STS_INCLUDE__ */
