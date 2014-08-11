#include "md_init.h"
#include "md_sts.h"
#include "md_type.h"
struct mpp_shared_data *gsdata;

int md_init(void)
{
    md_if_init();
    mpp_ifgrp_init();
    md_enabrave_init();
    InitQueue(&gsdata->pQ);
    md_rule_work_table_init();
    md_sts_init();

    return MD_SUCCESS;
}
