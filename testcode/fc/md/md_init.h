#include "md_embrave.h"
#include "md_sts.h"
#include "md_ifgrp.h"
#ifndef __MD_INIT_INCLUDE__
#define __MD_INIT_INCLUDE__
typedef struct mpp_shared_data
{
    mpp_epif                epif;
    mpp_if_group            if_group;
    uint8_t                 input_num;
    queue                   pQ;
    md_pkt_sts              sts;
};
int md_init(void);
#endif /* end of __MD_INIT_INCLUDE__ */
