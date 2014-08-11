#include "md_if.h"
#include "md_config.h"
#include "md_type.h"
#include "md_init.h"
extern struct mpp_shared_data *gsdata;

int md_if_init(void)
{
    int i;
    mpp_epif_entry *epif_entry = NULL;
    memset((void *)&gsdata->epif, 0 , sizeof(mpp_epif));
    for( i = 0; i < MPP_MAX_IF_NUM; i++ )
    {
        epif_entry = &gsdata->epif.epif_data[i];
        epif_entry->online = 1;
        epif_entry->weight = 1;
        epif_entry->iifgrp = INVALID_IIFGRP;
    }
    return MD_SUCCESS;
}

int if_get_if_online( uint8_t ipif, uint8_t *online)
{
    if ( ipif > MPP_MAX_IF_NUM - 1 )
    {
        return MD_PARAM;
    }

    *online =  gsdata->epif.epif_data[ipif].online;

    return MD_SUCCESS;
}

int if_set_if_online ( uint8_t ipif, uint8_t online )
{
    if ( ipif > MPP_MAX_IF_NUM - 1 )
    {
        return MD_PARAM;
    }

    gsdata->epif.epif_data[ipif].online = online;
    return MD_SUCCESS;
}

int if_get_if_phy( uint8_t ipif, uint8_t *link)
{
    if ( ipif > MPP_MAX_IF_NUM - 1 )
    {
        return MD_PARAM;
    }

    *link =  gsdata->epif.epif_data[ipif].online;

    return MD_SUCCESS;
}

int if_set_if_phy( uint8_t ipif, uint8_t link)
{
    if ( ipif > MPP_MAX_IF_NUM - 1 )
    {
        return MD_PARAM;
    }

    gsdata->epif.epif_data[ipif].online = link;
    return MD_SUCCESS;
}

int if_get_if_link( uint8_t ipif, uint8_t *link)
{
    if ( ipif > MPP_MAX_IF_NUM - 1 )
    {
        return MD_PARAM;
    }

    *link =  gsdata->epif.epif_data[ipif].link;

    return MD_SUCCESS;
}

int if_set_if_link ( uint8_t ipif, uint8_t flag )
{
    if ( ipif > MPP_MAX_IF_NUM - 1 )
    {
        return MD_PARAM;
    }
    if ( flag )
    {
        gsdata->epif.epif_data[ipif].link = 1;
    }
    else
    {
        gsdata->epif.epif_data[ipif].link = 0;
    }
    return MD_SUCCESS;
}

int if_get_if_iifgrp( uint8_t ipif, uint8_t *ifgrp)
{
    if ( ipif > MPP_MAX_IF_NUM - 1 )
    {
        return MD_PARAM;
    }

    *ifgrp =  gsdata->epif.epif_data[ipif].iifgrp;

    return MD_SUCCESS;
}

int if_set_if_iifgrp ( uint8_t ipif, uint8_t ifgrp )
{
    if ( ipif > MPP_MAX_IF_NUM - 1 )
    {
        return MD_PARAM;
    }

    gsdata->epif.epif_data[ipif].iifgrp = ifgrp;
    return MD_SUCCESS;
}

int if_get_if_weight( uint8_t ipif, uint8_t *weight)
{
    if ( ipif > MPP_MAX_IF_NUM - 1 )
    {
        return MD_PARAM;
    }

    *weight =  gsdata->epif.epif_data[ipif].weight;

    return MD_SUCCESS;
}

int if_set_if_weight ( uint8_t ipif, uint8_t weight )
{
    if ( ipif > MPP_MAX_IF_NUM - 1 )
    {
        return MD_PARAM;
    }

    gsdata->epif.epif_data[ipif].weight = weight;
    return MD_SUCCESS;
}

void md_if_status_loop(void)
{
    int i;
    uint8_t phy_sts; /* phyiscial status */
    uint8_t link;    /* saved status */
    int rv;
    while (1)
    {
        for ( i = 0; i < MPP_MAX_IF_NUM; i++)
        {
            /* get interface physical status */
            rv = if_get_if_phy(i, &phy_sts);
            if ( rv)
            {
                continue;
            }

            /* get saved link status */
            rv = if_get_if_link(i, &link);
            if ( rv)
            {
                continue;
            }

            if ( phy_sts == link )
            {
                continue;
            }

            rv = md_update_oifgrp_up_port_array(i, phy_sts);
            if (rv)
            {
                continue;
            }

            /* set saved link status */
            rv = if_set_if_link(i, phy_sts);
            if ( rv)
            {
                continue;
            }
        }
        sleep(1);
    }
}
