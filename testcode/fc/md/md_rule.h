#include "md_config.h"

#ifndef __MD_RULE_INCLUDE__
#define __MD_RULE_INCLUDE__


typedef enum
{
    MD_PKT_TYPE_UNKNOWN,
    MD_PKT_TYPE_ETH,
    MD_PKT_TYPE_ETH_IP,
    MD_PKT_TYPE_ETH_IP_TCP,
    MD_PKT_TYPE_ETH_IP_UDP,
    MD_PKT_TYPE_ETH_IP_SCTP,
}md_pkt_type;

int md_rule_work_table_init(void);
#endif /* end of __MD_RULE_INCLUDE__ */
