#include "md_if.h"
#include "md_wqe.h"
#include "md_init.h"
#include "md_config.h"
#include "md_type.h"
#include "md_rule.h"
#include "md_ifgrp.h"

extern struct mpp_shared_data *gsdata;

int md_sts_init(void)
{
    gsdata->sts.get = 0;
    gsdata->sts.free = 0;

    return MD_SUCCESS;
}

void md_sts_get_add(void)
{
    gsdata->sts.get ++;
}

void md_sts_free_add(void)
{
    gsdata->sts.free ++;
}

void md_sts_fwd_add(void)
{
    gsdata->sts.fwd ++;
}

void md_sts_drop_add(void)
{
    gsdata->sts.drop ++;
}

void md_sts_permit_add(void)
{
    gsdata->sts.permit ++;
}

int md_sts_clear(void )
{
    memset((void *)&gsdata->sts, 0, sizeof(md_pkt_sts));
    return MD_SUCCESS;
}

int  md_sts_get(md_pkt_sts *sts)
{
    if (!sts)
    {
        return MD_PARAM;
    }

    memcpy((void *)sts, (void *)&gsdata->sts, sizeof(md_pkt_sts));

    return MD_SUCCESS;
}
