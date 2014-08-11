#include "md_if.h"
#include "md_wqe.h"
#include "md_init.h"
#include "md_config.h"
#include "md_type.h"
#include "md_rule.h"
#include "md_ifgrp.h"
#include "md_sts.h"

extern struct mpp_shared_data *gsdata;

typedef enum
{
    MD_RULE_ACTION_DROP   = 1,
    MD_RULE_ACTION_FWD    = 2,
    MD_RULE_ACTION_MIRROR = 4,
    MD_RULE_ACTION_COPYTO = 8,
    MD_RULE_ACTION_PERMIT = 16,
}md_rule_action;

int md_rule_acl_work(cvmx_wqe_t *work, struct mpp_control *mpp)
{
    mpp->action = MD_RULE_ACTION_PERMIT;
    return MD_SUCCESS;
}

int md_rule_ip_work(cvmx_wqe_t *work, struct mpp_control *mpp)
{
    mpp->action = MD_RULE_ACTION_PERMIT;
    return MD_SUCCESS;
}

int md_rule_string_work(cvmx_wqe_t *work, struct mpp_control *mpp)
{
    mpp->action = MD_RULE_ACTION_PERMIT;
    return MD_SUCCESS;
}

int md_rule_payload_work(cvmx_wqe_t *work, struct mpp_control *mpp)
{
    mpp->action = MD_RULE_ACTION_PERMIT;
    return MD_SUCCESS;
}

int md_rule_work_table_init(void)
{
    int rv;
    rv = md_ifgrp_work_register(MD_IF_GRP_WORK_ACL, md_rule_acl_work);
    if ( rv)
    {
        return rv;
    }

    rv = md_ifgrp_work_register(MD_IF_GRP_WORK_IP, md_rule_ip_work);
    if ( rv)
    {
        return rv;
    }

    rv = md_ifgrp_work_register(MD_IF_GRP_WORK_STRING, md_rule_string_work);
    if ( rv)
    {
        return rv;
    }

    rv = md_ifgrp_work_register(MD_IF_GRP_WORK_PAYLOAD, md_rule_payload_work);
    if ( rv)
    {
        return rv;
    }

    return MD_SUCCESS;
}

int md_rule_work_copy_to_process(cvmx_wqe_t *work, struct mpp_control *mpp, uint8_t ifgrp)
{
    int rv;
    if_grp_work     *grp_work     = NULL;

    grp_work = md_ifgrp_get_copytogrp_work(ifgrp);
    if ( grp_work == NULL )
    {
        return MD_INIT;
    }
    if ( mpp->copyto_times > 5)
    {
        mpp->action = MD_RULE_ACTION_DROP;
    }
    mpp->copyto_times++;
    return md_rule_work_process(grp_work, work, mpp);
}

int md_rule_work_process(if_grp_work *grp_work, cvmx_wqe_t *work, struct mpp_control *mpp)
{
    int i = 0;
    int rv = MD_SUCCESS;
    if_grp_acl_callback *acl_callback = NULL;
    acl_callback = grp_work->md_acl;
    //while ( acl_callback[i] != NULL)
    while ( grp_work->grp_acl[grp_work->master][i] != NULL)
    {
        //rv = (acl_callback[i])(work, mpp);
        rv = (grp_work->grp_acl[grp_work->master][i])(work, mpp);
        if (rv )
        {
            mpp->action = MD_RULE_ACTION_DROP;
            md_sts_drop_add();
            break;
        }
        switch (mpp->action)
        {
            case MD_RULE_ACTION_PERMIT:
                md_sts_permit_add();
                break;
            case MD_RULE_ACTION_FWD:
                md_sts_fwd_add();
                break;
            case MD_RULE_ACTION_DROP:
                md_sts_drop_add();
                break;
            case MD_RULE_ACTION_COPYTO:
                break;
            case MD_RULE_ACTION_MIRROR:
                break;
            default :
                break;
        }
        if ( mpp->action == MD_RULE_ACTION_PERMIT )
        {
            i++;
        }
        else
        {
            break;
        }
    }

    return rv;
}

int rule_work(cvmx_wqe_t *work, struct mpp_control *mpp)
{
    int rv;
    uint8_t ifgrp;
    if_grp_work     *grp_work     = NULL;
    /* get interface from wqe */
    mpp->ipif = work->port;
    mpp->action = MD_RULE_ACTION_DROP;

    /* get input interface group from epif */
    rv = if_get_if_iifgrp( mpp->ipif, &ifgrp);
    if (rv)
    {
        return rv;
    }
    printf("%s %d ifgrp=%d\n", __func__, __LINE__, ifgrp);
    if (ifgrp == INVALID_IIFGRP)
    {
        return MD_SUCCESS;
    }

    if ( ifgrp < IF_GROUP_NUM )
    {
        grp_work = md_ifgrp_get_iifgrp_work(ifgrp);
    }
    else
    {
        grp_work = md_ifgrp_get_copytogrp_work(ifgrp - IF_GROUP_NUM);
    }

    if ( grp_work == NULL )
    {
        return MD_INIT;
    }

    printf("%s %d grp_work=%p, 0x%x\n", __func__, __LINE__, grp_work, grp_work);
    return md_rule_work_process(grp_work, work, mpp);
}

static int mc_main_protocol_analysis ( cvmx_wqe_t *work,  struct mpp_control *mpp )
{
    mpp->pkt_type = MD_PKT_TYPE_ETH_IP;
    return MD_SUCCESS;
}

static int mc_main_procs_packet_data ( cvmx_wqe_t *work,  struct mpp_control *mpp )
{
    int rv;
    rv = mc_main_protocol_analysis(work, mpp);
    if (rv)
    {
        printf("%s %d rv=%d\n", __func__, __LINE__, rv);
        return rv;
    }

    switch (mpp->pkt_type)
    {
        case MD_PKT_TYPE_ETH:
            break;
        case MD_PKT_TYPE_ETH_IP:
            rule_work(work, mpp);
            break;
        case MD_PKT_TYPE_ETH_IP_TCP:
            break;
        case MD_PKT_TYPE_ETH_IP_UDP:
            break;
        default :
            break;
    }

    return MD_SUCCESS;
}

static inline void process_work ( cvmx_wqe_t *work )
{
    struct mpp_control *mpp = NULL;
    mpp = ( struct mpp_control * ) ( &work->packet_data[0] );
    mc_main_procs_packet_data ( work, mpp );
}

int application_main_loop(void)
{
    cvmx_wqe_t *work = NULL;

    while (1)
    {
        work = cvmx_pow_work_request_sync();

        if (work == NULL)
        {
            continue;
        }
        else
        {
            md_sts_get_add();
            process_work ( work );
            mpp_free_wqe_packet_data (work);
            md_sts_free_add();
        }
    }

    return MD_SUCCESS;
}
