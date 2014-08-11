#include "md_ifgrp.h"
#include "md_type.h"
#include "md_init.h"

extern struct mpp_shared_data *gsdata;

/* get  from global share data */
mpp_epif_entry * md_ifgrp_get_epif(uint8_t if_id)
{
    if (!gsdata)
    {
        return NULL;
    }

    return &gsdata->epif.epif_data[if_id];
}

/* get iifgrp from global share data */
iif_grp *md_ifgrp_get_iifgrp(uint8_t grp_id)
{
    if (!gsdata)
    {
        return NULL;
    }

    return &gsdata->if_group.iif_group[grp_id];
}

/* get oifgrp from global share data */
oif_grp *md_ifgrp_get_oifgrp(uint8_t grp_id)
{
    if (!gsdata)
    {
        return NULL;
    }

    return &gsdata->if_group.oif_group[grp_id];
}

/* get copy-to group from global share data */
copyto_grp *md_ifgrp_get_copytogrp(uint8_t grp_id)
{
    if (!gsdata)
    {
        return NULL;
    }

    return &gsdata->if_group.copyto_group[grp_id];
}

/* get iifgrp if_grp_work from global share data */
if_grp_work *md_ifgrp_get_iifgrp_work(uint8_t grp_id)
{
    if (!gsdata)
    {
        return NULL;
    }

    return &gsdata->if_group.iif_group[grp_id].grp_work;
}

/* get copy-to group from global share data */
if_grp_work *md_ifgrp_get_copytogrp_work(uint8_t grp_id)
{
    if (!gsdata)
    {
        return NULL;
    }

    return &gsdata->if_group.copyto_group[grp_id].grp_work;
}

/* get interface group work table from global share data */
if_grp_work_table *md_ifgrp_get_work_table(uint8_t priority)
{
    if (!gsdata)
    {
        return NULL;
    }

    return &gsdata->if_group.work_table[priority];
}

/* get interface group work table from global share data */
if_grp_work_table *md_ifgrp_get_work_table_head(void)
{
    if (!gsdata)
    {
        return NULL;
    }

    return gsdata->if_group.work_table;
}

int md_if_grp_search_work_table(if_grp_acl_callback acl_callback, uint8_t *index)
{
    if_grp_work_table  *work_table = NULL;
    int i;

    if ( acl_callback == NULL )
    {
        return MD_PARAM;
    }

    if ( index == NULL )
    {
        return MD_PARAM;
    }

    work_table = md_ifgrp_get_work_table_head();
    if ( work_table == NULL)
    {
        return MD_INIT;
    }

    *index = INVALID_IFGRP_WORK_INDEX;
    for ( i = 0; i < (int) MD_IF_GRP_WORK_MAX; i++)
    {
        if ( work_table[i].acl_callback == acl_callback )
        {
            *index = i;
            break;
        }
    }

    return MD_SUCCESS;
}

int  mpp_ifgrp_init(void)
{
    iif_grp        *iifgrp  = NULL;
    oif_grp        *oifgrp  = NULL;
    copyto_grp     *copytogrp = NULL;
    if_grp_work_table *work_table = NULL;
    int i,j,k;
    /* iifgrp global data init */
    for ( i = 0; i < IF_GROUP_NUM; i++)
    {
        iifgrp = md_ifgrp_get_iifgrp(i);
        if (iifgrp == NULL)
        {
            return MD_INIT;
        }
        iifgrp->if_grp_id = i;
        iifgrp->grp_member.if_grp_port_num = 0;
        memset((void *)iifgrp->grp_member.ifmask, 0, sizeof(iifgrp->grp_member.ifmask));
        iifgrp->grp_work.mdu_acl = NULL;
        iifgrp->grp_work.md_acl = iifgrp->grp_work.grp_acl[0];
        iifgrp->grp_work.master = 0;
        for ( j = 0; j < 2; j++ )
        {
            for ( k = 0; k < MAX_GRP_ACL; k++ )
            {
                iifgrp->grp_work.grp_acl[j][k] = NULL;
            }
        }

        for ( j = 0; j < MAX_GRP_ACL; j++ )
        {
            iifgrp->grp_work.priority[j] = INVALID_IFGRP_WORK_INDEX;
        }
    }

    /* oifgrp global data init */
    for ( i = 0; i < IF_GROUP_NUM; i++)
    {
        oifgrp = md_ifgrp_get_oifgrp(i);
        if (oifgrp == NULL)
        {
            return MD_INIT;
        }
        oifgrp->if_grp_id = i;
        oifgrp->port_num = 0;
        oifgrp->dn_port_num = 0;
        memset((void *)oifgrp->ifmask, 0, sizeof(oifgrp->ifmask));
        memset((void *)oifgrp->up_ifmask, 0, sizeof(oifgrp->up_ifmask));
        oifgrp->mode = OIF_GRP_MODE_ROUND_ROBIN;
        oifgrp->hash = OIF_GRP_HASH_SIP;
        for ( j = 0; j < 2; j++ )
        {
            oifgrp->up_port[j].up_port_num = 0;
            for ( k = 0; k < OIF_GRP_UP_PORT_MEMBER_MAX; k++ )
            {
                oifgrp->up_port[j].up_port[k] = 0;
            }
        }
    }

    /* copy-to gloabl data init */
    for ( i = 0; i < IF_GROUP_NUM; i++)
    {
        copytogrp = md_ifgrp_get_copytogrp(i);
        if (copytogrp == NULL)
        {
            return MD_INIT;
        }

        copytogrp->if_grp_id = i;
        copytogrp->grp_work.mdu_acl = NULL;
        copytogrp->grp_work.md_acl = copytogrp->grp_work.grp_acl[0];
        copytogrp->grp_work.master = 0;
        for ( j = 0; j < 2; j++ )
        {
            for ( k = 0; k < MAX_GRP_ACL; k++ )
            {
                copytogrp->grp_work.grp_acl[j][k] = NULL;
            }
        }

        for ( j = 0; j < MAX_GRP_ACL; j++ )
        {
            copytogrp->grp_work.priority[j] = INVALID_IFGRP_WORK_PRIORITY;
        }
    }

    for ( i = 0; i < MD_IF_GRP_WORK_MAX; i++ )
    {
        work_table = md_ifgrp_get_work_table(i);
        if (work_table == NULL)
        {
            return MD_INIT;
        }
        work_table->priority = INVALID_IFGRP_WORK_INDEX;
        work_table->acl_callback = NULL;
    }
    return MD_SUCCESS;
}

int md_ifgrp_work_register(md_ifgrp_work_t work, if_grp_acl_callback acl_callback)
{
    if_grp_work_table *work_table = NULL;

    work_table = md_ifgrp_get_work_table(work);
    if (work_table == NULL)
    {
        return MD_INIT;
    }

    if (work_table->priority == INVALID_IFGRP_WORK_PRIORITY)
    {
        work_table->priority = work;
        work_table->acl_callback = acl_callback;
    }
    else
    {
        return MD_EXIST;
    }

    return MD_SUCCESS;
}

/* md input interface group operate function */
int md_iifgrp_add(uint8_t grp_id, uint8_t if_id)
{
    mpp_epif_entry *epif = NULL;
    iif_grp        *grp  = NULL;
    int index, bit;
    /* check parameter */
    if ( (grp_id >= IF_GROUP_NUM) || (if_id >= MAX_IF_NUM))
    {
        return MD_PARAM;
    }

    grp = md_ifgrp_get_iifgrp(grp_id);
    if ( grp == NULL )
    {
        return MD_MEMORY;
    }

    epif = md_ifgrp_get_epif(if_id);
    if ( epif == NULL )
    {
        return MD_MEMORY;
    }

    /* if interface has joined to a group, return */
    if ( epif->iifgrp != INVALID_IIFGRP )
    {
        return MD_EXIST;
    }

    index = if_id / UINT64_BIT_NUM;
    bit   = if_id % UINT64_BIT_NUM;
    if ( grp->grp_member.ifmask[index] & (1 << (UINT64_BIT_NUM - bit -1)) )
    {
        return MD_EXIST;
    }

    grp->grp_member.ifmask[index] |= 1 << (UINT64_BIT_NUM - bit -1);
    grp->grp_member.if_grp_port_num ++;

    epif->iifgrp = grp_id;

    return MD_SUCCESS;
}

int md_iifgrp_del(uint8_t grp_id, uint8_t if_id)
{
    mpp_epif_entry *epif = NULL;
    iif_grp        *grp  = NULL;
    int index, bit;
    /* check parameter */
    if ( (grp_id >= IF_GROUP_NUM) || (if_id >= MAX_IF_NUM))
    {
        return MD_PARAM;
    }

    grp = md_ifgrp_get_iifgrp(grp_id);
    if ( grp == NULL )
    {
        return MD_MEMORY;
    }

    epif = md_ifgrp_get_epif(if_id);
    if ( epif == NULL )
    {
        return MD_MEMORY;
    }

    /* if interface has joined to a group, return */
    if ( epif->iifgrp != grp_id )
    {
        return MD_UNMATCH;
    }

    index = if_id / UINT64_BIT_NUM;
    bit   = if_id % UINT64_BIT_NUM;
    if ( !(grp->grp_member.ifmask[index] & (1 << (UINT64_BIT_NUM - bit -1))) )
    {
        return MD_NOTFOUND;
    }

    grp->grp_member.ifmask[index] &= ~((uint64_t)(1 << (UINT64_BIT_NUM - bit -1)));
    grp->grp_member.if_grp_port_num --;

    epif->iifgrp = INVALID_IIFGRP;

    return MD_SUCCESS;
}

int md_iifgrp_get(uint8_t grp_id, iif_grp *grp)
{
    iif_grp        *ifgrp  = NULL;

    /* check parameter */
    if ( grp_id >= IF_GROUP_NUM )
    {
        return MD_PARAM;
    }

    if ( grp == NULL)
    {
        return MD_PARAM;
    }

    memset((void *)grp, 0, sizeof(iif_grp));

    ifgrp = md_ifgrp_get_iifgrp(grp_id);
    if ( ifgrp == NULL )
    {
        return MD_MEMORY;
    }

    memcpy((void *)grp, (void *)ifgrp, sizeof(iif_grp));

    return MD_SUCCESS;
}
#ifndef PLATFORM_X86
#define MD_PTR_TO_PHYS(ptr) cvmx_ptr_to_phys(ptr)
#else
#define MD_PTR_TO_PHYS(ptr) (ptr)
#endif
int md_iifgrp_bind_work(uint8_t grp_id, md_ifgrp_work_t work, int priority)
{
    int i, j;
    int master,backup;
    iif_grp        *grp  = NULL;
    if_grp_work_table *work_table = NULL;
    if_grp_work    *grp_work;

    /* check parameter */
    if ( grp_id >= IF_GROUP_NUM )
    {
        return MD_PARAM;
    }

    if (priority >= MD_IF_GRP_WORK_MAX)
    {
        return MD_PARAM;
    }

    grp = md_ifgrp_get_iifgrp(grp_id);
    if ( grp == NULL )
    {
        return MD_MEMORY;
    }

    work_table = md_ifgrp_get_work_table_head();
    if (work_table == NULL)
    {
        return MD_INIT;
    }

    if ( work_table->priority == INVALID_IFGRP_WORK_PRIORITY ||
            work_table->acl_callback == NULL)
    {
        return MD_INIT;
    }

    grp_work = &grp->grp_work;

    /* check current work priority whether effective or not */
    if ( grp_work->priority[priority] != INVALID_IFGRP_WORK_INDEX)
    {
        return MD_EXIST;
    }

    /* search work whether existed or not*/
    for ( i = 0; i < MAX_GRP_ACL; i++ )
    {
        if (grp_work->priority[i] == work )
        {
            grp_work->priority[i] = INVALID_IFGRP_WORK_INDEX;
        }
    }
    
    /* modify priority */
    grp_work->priority[priority] = work;

    /* sort */
    master = grp_work->master;
    backup = !master;
    for ( i = 0; i < MAX_GRP_ACL; i++)
    {
        grp_work->grp_acl[backup][i] = NULL;
    }

    for ( i = 0, j = 0; i < MAX_GRP_ACL; i++)
    {
        if ( grp_work->priority[i] == INVALID_IFGRP_WORK_INDEX)
        {
            continue;
        }
        grp_work->grp_acl[backup][j++] =
            work_table[grp_work->priority[i]].acl_callback;
    }

    /* modify master */
    grp_work->master  = backup;
    grp_work->mdu_acl = grp_work->grp_acl[backup];
    grp_work->md_acl  = MD_PTR_TO_PHYS(grp_work->grp_acl[backup]);
    for ( i = 0; i < MAX_GRP_ACL; i++)
    {
        grp_work->grp_acl[master][i] = grp_work->grp_acl[backup][i];
    }

    return MD_SUCCESS;
}

int md_iifgrp_unbind_work(uint8_t grp_id, md_ifgrp_work_t work)
{
    int i, j;
    int master,backup;
    iif_grp        *grp  = NULL;
    if_grp_work_table *work_table = NULL;
    if_grp_work    *grp_work;
    int flag_exist = 0;

    /* check parameter */
    if ( grp_id >= IF_GROUP_NUM )
    {
        return MD_PARAM;
    }

    grp = md_ifgrp_get_iifgrp(grp_id);
    if ( grp == NULL )
    {
        return MD_MEMORY;
    }

    work_table = md_ifgrp_get_work_table_head();
    if (work_table == NULL)
    {
        return MD_INIT;
    }

    if ( work_table->priority == INVALID_IFGRP_WORK_PRIORITY ||
            work_table->acl_callback == NULL)
    {
        return MD_INIT;
    }

    grp_work = &grp->grp_work;

    /* search work whether existed or not*/
    for ( i = 0; i < MAX_GRP_ACL; i++ )
    {
        if (grp_work->priority[i] == work )
        {
            flag_exist = 1;
            break;
        }
    }

    if (!flag_exist)
    {
        return MD_NOTFOUND;
    }
    
    /* del work index*/
    grp_work->priority[i] = INVALID_IFGRP_WORK_INDEX;

    /* sort */
    master = grp_work->master;
    backup = !master;
    for ( i = 0; i < MAX_GRP_ACL; i++)
    {
        grp_work->grp_acl[backup][i] = NULL;
    }

    for ( i = 0, j = 0; i < MAX_GRP_ACL; i++)
    {
        if ( grp_work->priority[i] == INVALID_IFGRP_WORK_INDEX)
        {
            continue;
        }
        grp_work->grp_acl[backup][j++] =
            work_table[grp_work->priority[i]].acl_callback;
    }

    /* modify master */
    grp_work->master  = backup;
    grp_work->mdu_acl = grp_work->grp_acl[backup];
    grp_work->md_acl  = MD_PTR_TO_PHYS(grp_work->grp_acl[backup]);
    for ( i = 0; i < MAX_GRP_ACL; i++)
    {
        grp_work->grp_acl[master][i] = grp_work->grp_acl[backup][i];
    }
    return MD_SUCCESS;
}

int md_iifgrp_retore_work(uint8_t grp_id)
{
    int i, j;
    int master,backup;
    iif_grp        *grp  = NULL;
    if_grp_work_table *work_table = NULL;
    if_grp_work    *grp_work;
    uint8_t priority_backup[MAX_GRP_ACL];

    /* check parameter */
    if ( grp_id >= IF_GROUP_NUM )
    {
        return MD_PARAM;
    }

    grp = md_ifgrp_get_iifgrp(grp_id);
    if ( grp == NULL )
    {
        return MD_MEMORY;
    }

    work_table = md_ifgrp_get_work_table_head();
    if (work_table == NULL)
    {
        return MD_INIT;
    }

    grp_work = &grp->grp_work;

    /* back up priority */
    for ( i = 0; i < MAX_GRP_ACL; i++ )
    {
        priority_backup[i] = grp_work->priority[i];
    }

    for ( i = 0; i < MAX_GRP_ACL; i++ )
    {
        grp_work->priority[i] = INVALID_IFGRP_WORK_INDEX;
        if ( priority_backup[i] == INVALID_IFGRP_WORK_INDEX)
        {
            continue;
        }
        if ( priority_backup[i] != i)
        {
            grp_work->priority[priority_backup[i]] = priority_backup[i] ;
        }
        else
        {
        
            grp_work->priority[i] = i ;
        }
    }


    /* sort */
    master = grp_work->master;
    backup = !master;
    for ( i = 0; i < MAX_GRP_ACL; i++)
    {
        grp_work->grp_acl[backup][i] = NULL;
    }

    for ( i = 0, j = 0; i < MAX_GRP_ACL; i++)
    {
        if ( grp_work->priority[i] == INVALID_IFGRP_WORK_INDEX)
        {
            continue;
        }
        grp_work->grp_acl[backup][j++] =
            work_table[grp_work->priority[i]].acl_callback;
    }

    /* modify master */
    grp_work->master  = backup;
    grp_work->mdu_acl = grp_work->grp_acl[backup];
    grp_work->md_acl  = MD_PTR_TO_PHYS(grp_work->grp_acl[backup]);
    for ( i = 0; i < MAX_GRP_ACL; i++)
    {
        grp_work->grp_acl[master][i] = grp_work->grp_acl[backup][i];
    }
    return MD_SUCCESS;
}

int md_oifgrp_add_if_to_up_port_array(oif_grp_up_port_member *up_port, int if_id, int weight)
{
    int i, j, k;
    int move;
    uint32_t up_port_num;

    up_port_num = up_port->up_port_num;
    for ( i = 0; i < up_port_num; i++)
    {
        if (up_port->up_port[i] > if_id )
        {
            break;
        }
    }

    /* move number */
    move = up_port_num - i;
    for ( j = 0; j < move; j++)
    {
        up_port->up_port[up_port_num + weight - j - 1] =
            up_port->up_port[up_port_num - j -1] ;

    }

    /* fill into new interface into up port array */
    for ( k = 0; k < weight; k++)
    {
        up_port->up_port[i+k] = if_id;
    }
    up_port_num += weight;
    up_port->up_port_num = up_port_num;

    return MD_SUCCESS;
}

/* md output interface group operate function */
int md_oifgrp_add(uint8_t grp_id, uint8_t if_id)
{
    mpp_epif_entry *epif = NULL;
    oif_grp        *grp  = NULL;
    int index, bit;
    uint8_t link = 0;
    int rv;
    uint8_t master, backup;
    int i;
    oif_grp_up_port_member *up_port_master = NULL, *up_port_backup = NULL;
    int weight;

    /* check parameter */
    if ( (grp_id >= IF_GROUP_NUM) || (if_id >= MAX_IF_NUM))
    {
        return MD_PARAM;
    }

    grp = md_ifgrp_get_oifgrp(grp_id);
    if ( grp == NULL )
    {
        return MD_MEMORY;
    }

    epif = md_ifgrp_get_epif(if_id);
    if ( epif == NULL )
    {
        return MD_MEMORY;
    }

    index = if_id / UINT64_BIT_NUM;
    bit   = if_id % UINT64_BIT_NUM;
    if ( grp->ifmask[index] & (1 << (UINT64_BIT_NUM - bit -1)) )
    {
        return MD_EXIST;
    }

    grp->ifmask[index] |= 1 << (UINT64_BIT_NUM - bit -1);
    grp->port_num ++;

    rv = if_get_if_link(if_id, &link);
    if (rv)
    {
        return rv;
    }

    if (!link)
    {
        grp->dn_port_num ++;
        return MD_SUCCESS;
    }

    grp->up_ifmask[index] |= 1 << (UINT64_BIT_NUM - bit -1);

    master = grp->master;
    backup = !master;

    up_port_master = &grp->up_port[master];
    up_port_backup = &grp->up_port[backup];

    /* get interface weight */
    if (grp->mode == OIF_GRP_MODE_WEIGHT_ROUND_ROBIN ||
            grp->mode == OIF_GRP_MODE_WEIGHT_HASH)
    {
        weight = 10;
    }
    else
    {
        weight = 1;
    }

    /* add interface to up port array */
    rv =  md_oifgrp_add_if_to_up_port_array(up_port_backup, if_id, weight);
    if (rv)
    {
        return rv;
    }

    grp->master = backup;
    /* copy current up port array to old master array */
    up_port_master->up_port_num = up_port_backup->up_port_num;
    for ( i = 0; i < up_port_master->up_port_num; i++)
    {
        up_port_master->up_port[i] = up_port_backup->up_port[i];
    }

    return MD_SUCCESS;
}

int md_oifgrp_del_if_from_up_port_array(oif_grp_up_port_member *up_port, int if_id, int weight)
{
    uint32_t up_port_num;
    int i, j;
    int move;

    up_port_num = up_port->up_port_num;
    for ( i = 0; i < up_port_num; i++)
    {
        if (up_port->up_port[i] > if_id )
        {
            break;
        }
    }

    /* move number */
    move = up_port_num - i;

    /* move interface forward */
    for ( j = 0; j < move; j++)
    {
        up_port->up_port[i + j] =
            up_port->up_port[ i + weight + j];
    }

    up_port_num -= weight;
    up_port->up_port_num = up_port_num;

    return MD_SUCCESS;
}

int md_oifgrp_del(uint8_t grp_id, uint8_t if_id)
{
    mpp_epif_entry *epif = NULL;
    oif_grp        *grp  = NULL;
    int index, bit;
    uint8_t master, backup;
    int i, j, k;
    uint32_t up_prort_num;
    oif_grp_up_port_member *up_port_master = NULL, *up_port_backup = NULL;
    int weight;
    int rv;

    /* check parameter */
    if ( (grp_id >= IF_GROUP_NUM) || (if_id >= MAX_IF_NUM))
    {
        return MD_PARAM;
    }

    grp = md_ifgrp_get_oifgrp(grp_id);
    if ( grp == NULL )
    {
        return MD_MEMORY;
    }

    epif = md_ifgrp_get_epif(if_id);
    if ( epif == NULL )
    {
        return MD_MEMORY;
    }

    index = if_id / UINT64_BIT_NUM;
    bit   = if_id % UINT64_BIT_NUM;
    if ( !(grp->ifmask[index] & (1 << (UINT64_BIT_NUM - bit -1))) )
    {
        return MD_UNMATCH;
    }

    /* clear if_mask flag */
    grp->ifmask[index] &= ~((uint64_t)(1 << (UINT64_BIT_NUM - bit -1)));
    grp->port_num --;

    /* check up if_mask flag */
    if ( !(grp->up_ifmask[index] & (1 << (UINT64_BIT_NUM - bit -1))) )
    {
        return MD_SUCCESS;
    }

    /* clear up if_mask flag */
    grp->up_ifmask[index] &= ~((uint64_t)(1 << (UINT64_BIT_NUM - bit -1)));

    master = grp->master;
    backup = !master;

    up_port_master = &grp->up_port[master];
    up_port_backup = &grp->up_port[backup];

    /* get interface weight */
    if (grp->mode == OIF_GRP_MODE_WEIGHT_ROUND_ROBIN ||
            grp->mode == OIF_GRP_MODE_WEIGHT_HASH)
    {
        weight = 10;
    }
    else
    {
        weight = 1;
    }

    /* del interface from up port array */
    rv = md_oifgrp_del_if_from_up_port_array(up_port_backup, if_id, weight);
    if (rv)
    {
        return rv;
    }

    grp->master = backup;

    /* copy current up port array to old master array */
    up_port_master->up_port_num = up_port_backup->up_port_num;
    for ( i = 0; i < up_port_master->up_port_num; i++)
    {
        up_port_master->up_port[i] = up_port_backup->up_port[i];
    }

    return MD_SUCCESS;
}

int md_oifgrp_get(uint8_t grp_id, oif_grp *grp)
{
    oif_grp        *ifgrp  = NULL;

    /* check parameter */
    if ( grp_id >= IF_GROUP_NUM )
    {
        return MD_PARAM;
    }

    if ( grp == NULL)
    {
        return MD_PARAM;
    }

    memset((void *)grp, 0, sizeof(oif_grp));

    ifgrp = md_ifgrp_get_oifgrp(grp_id);
    if ( ifgrp == NULL )
    {
        return MD_MEMORY;
    }

    memcpy((void *)grp, (void *)ifgrp, sizeof(oif_grp));
    return MD_SUCCESS;
}

/* modify interface weight*/
int md_oifgrp_modify_if_to_up_port_array(oif_grp_up_port_member *up_port, int if_id, int weight_new, int weight_old)
{
    int rv;
    rv = md_oifgrp_del_if_from_up_port_array(up_port, if_id, weight_old);
    if ( rv )
    {
        return rv;
    }

    rv = md_oifgrp_add_if_to_up_port_array(up_port, if_id, weight_new);
    if ( rv )
    {
        return rv;
    }

    return MD_SUCCESS;
}

int md_oifgrp_set_mode(uint8_t grp_id, oif_grp_mode mode)
{
    //mpp_epif_entry *epif = NULL;
    oif_grp        *grp  = NULL;
    int index, bit;
    int rv;
    uint8_t master, backup;
    int i;
    oif_grp_up_port_member *up_port_master = NULL, *up_port_backup = NULL;
    int weight_new, weight_old;
    int weight_flag;

    /* check parameter */
    if ( grp_id >= IF_GROUP_NUM )
    {
        return MD_PARAM;
    }

    grp = md_ifgrp_get_oifgrp(grp_id);
    if ( grp == NULL )
    {
        return MD_MEMORY;
    }

    //epif = md_ifgrp_get_epif(if_id);
    //if ( epif == NULL )
    //{
    //    return MD_MEMORY;
    //}

    /* if same with old mode, return */
    switch (mode)
    {
        case OIF_GRP_MODE_ROUND_ROBIN:
        case OIF_GRP_MODE_HASH:
            if ( grp->mode == OIF_GRP_MODE_ROUND_ROBIN ||
                    grp->mode == OIF_GRP_MODE_HASH )
            {
                grp->mode = mode;
                return MD_SUCCESS;
            }
            weight_flag = 0;
            break;
        case OIF_GRP_MODE_WEIGHT_ROUND_ROBIN:
        case OIF_GRP_MODE_WEIGHT_HASH:
            if ( grp->mode == OIF_GRP_MODE_WEIGHT_ROUND_ROBIN ||
                    grp->mode == OIF_GRP_MODE_WEIGHT_HASH )
            {
                grp->mode = mode;
                return MD_SUCCESS;
            }
            weight_flag = 1;
            break;
        default :
            break;
    }

    master = grp->master;
    backup = !master;
    up_port_master = &grp->up_port[master];
    up_port_backup = &grp->up_port[backup];

    for ( i = 0; i < MAX_IF_NUM; i++)
    {
        index = i / UINT64_BIT_NUM;
        bit   = i % UINT64_BIT_NUM;
        if ( !(grp->up_ifmask[index] & (1 << (UINT64_BIT_NUM - bit -1))) )
        {
            continue;
        }

        /* get interface weight */
        if (weight_flag)
        {
            weight_new = 10;
            weight_old = 1;
        }
        else
        {
            weight_new = 1;
            weight_old = 10;
        }

        rv = md_oifgrp_modify_if_to_up_port_array(up_port_backup, i, weight_new, weight_old);
        if (rv)
        {
            return rv;
        }
    }

    grp->master = backup;
    /* copy current up port array to old master array */
    up_port_master->up_port_num = up_port_backup->up_port_num;
    for ( i = 0; i < up_port_master->up_port_num; i++)
    {
        up_port_master->up_port[i] = up_port_backup->up_port[i];
    }

    grp->mode = mode;
    return MD_SUCCESS;
}

int md_oifgrp_set_hash(uint8_t grp_id, oif_grp_hash hash)
{
    oif_grp        *grp  = NULL;

    /* check parameter */
    if ( grp_id >= IF_GROUP_NUM )
    {
        return MD_PARAM;
    }

    grp = md_ifgrp_get_oifgrp(grp_id);
    if ( grp == NULL )
    {
        return MD_MEMORY;
    }


    grp->hash = hash;

    return MD_SUCCESS;
}

int md_cifgrp_get(uint8_t grp_id, copyto_grp *grp)
{
    copyto_grp        *ifgrp  = NULL;

    /* check parameter */
    if ( grp_id >= IF_GROUP_NUM )
    {
        return MD_PARAM;
    }

    if ( grp == NULL)
    {
        return MD_PARAM;
    }

    memset((void *)grp, 0, sizeof(iif_grp));

    ifgrp = md_ifgrp_get_copytogrp(grp_id);
    if ( ifgrp == NULL )
    {
        return MD_MEMORY;
    }

    memcpy((void *)grp, (void *)ifgrp, sizeof(copyto_grp));

    return MD_SUCCESS;
}

int md_cifgrp_bind_work(uint8_t grp_id, md_ifgrp_work_t work, int priority)
{
    int i, j;
    int master,backup;
    copyto_grp        *grp  = NULL;
    if_grp_work_table *work_table = NULL;
    if_grp_work    *grp_work;

    /* check parameter */
    if ( grp_id >= IF_GROUP_NUM )
    {
        return MD_PARAM;
    }

    if (priority >= MD_IF_GRP_WORK_MAX)
    {
        return MD_PARAM;
    }

    grp = md_ifgrp_get_copytogrp(grp_id);
    if ( grp == NULL )
    {
        return MD_MEMORY;
    }

    work_table = md_ifgrp_get_work_table_head();
    if (work_table == NULL)
    {
        return MD_INIT;
    }

    if ( work_table->priority == INVALID_IFGRP_WORK_INDEX ||
            work_table->acl_callback == NULL)
    {
        return MD_INIT;
    }

    grp_work = &grp->grp_work;

    /* check current work priority whether effective or not */
    if ( grp_work->priority[priority] != INVALID_IFGRP_WORK_INDEX)
    {
        return MD_EXIST;
    }

    /* search work whether existed or not*/
    for ( i = 0; i < MAX_GRP_ACL; i++ )
    {
        if (grp_work->priority[i] == work )
        {
            grp_work->priority[i] = INVALID_IFGRP_WORK_INDEX;
        }
    }
    
    /* modify priority */
    grp_work->priority[priority] = work;

    /* sort */
    master = grp_work->master;
    backup = !master;
    for ( i = 0; i < MAX_GRP_ACL; i++)
    {
        grp_work->grp_acl[backup][i] = NULL;
    }

    for ( i = 0, j = 0; i < MAX_GRP_ACL; i++)
    {
        if ( grp_work->priority[i] == INVALID_IFGRP_WORK_INDEX)
        {
            continue;
        }
        grp_work->grp_acl[backup][j++] =
            work_table[grp_work->priority[i]].acl_callback;
    }

    /* modify master */
    grp_work->master  = backup;
    grp_work->mdu_acl = grp_work->grp_acl[backup];
    grp_work->md_acl  = MD_PTR_TO_PHYS(grp_work->grp_acl[backup]);
    for ( i = 0; i < MAX_GRP_ACL; i++)
    {
        grp_work->grp_acl[master][i] = grp_work->grp_acl[backup][i];
    }

    return MD_SUCCESS;
}

int md_cifgrp_unbind_work(uint8_t grp_id, md_ifgrp_work_t work)
{
    int i, j;
    int master,backup;
    copyto_grp        *grp  = NULL;
    if_grp_work_table *work_table = NULL;
    if_grp_work    *grp_work;
    int flag_exist = 0;

    /* check parameter */
    if ( grp_id >= IF_GROUP_NUM )
    {
        return MD_PARAM;
    }

    grp = md_ifgrp_get_copytogrp(grp_id);
    if ( grp == NULL )
    {
        return MD_MEMORY;
    }

    work_table = md_ifgrp_get_work_table_head();
    if (work_table == NULL)
    {
        return MD_INIT;
    }

    if ( work_table->priority == INVALID_IFGRP_WORK_PRIORITY ||
            work_table->acl_callback == NULL)
    {
        return MD_INIT;
    }

    grp_work = &grp->grp_work;

    /* search work whether existed or not*/
    for ( i = 0; i < MAX_GRP_ACL; i++ )
    {
        if (grp_work->priority[i] == work )
        {
            flag_exist = 1;
            break;
        }
    }

    if (!flag_exist)
    {
        return MD_NOTFOUND;
    }

    /* del work index*/
    grp_work->priority[i] = INVALID_IFGRP_WORK_INDEX;

    /* sort */
    master = grp_work->master;
    backup = !master;
    for ( i = 0; i < MAX_GRP_ACL; i++)
    {
        grp_work->grp_acl[backup][i] = NULL;
    }

    for ( i = 0, j = 0; i < MAX_GRP_ACL; i++)
    {
        if ( grp_work->priority[i] == INVALID_IFGRP_WORK_INDEX)
        {
            continue;
        }
        grp_work->grp_acl[backup][j++] =
            work_table[grp_work->priority[i]].acl_callback;
    }

    /* modify master */
    grp_work->master  = backup;
    grp_work->mdu_acl = grp_work->grp_acl[backup];
    grp_work->md_acl  = MD_PTR_TO_PHYS(grp_work->grp_acl[backup]);
    for ( i = 0; i < MAX_GRP_ACL; i++)
    {
        grp_work->grp_acl[master][i] = grp_work->grp_acl[backup][i];
    }
    return MD_SUCCESS;
}

int md_cifgrp_retore_work(uint8_t grp_id)
{
    int i, j;
    int master,backup;
    copyto_grp        *grp  = NULL;
    if_grp_work_table *work_table = NULL;
    if_grp_work    *grp_work;
    uint8_t priority_backup[MAX_GRP_ACL];

    /* check parameter */
    if ( grp_id >= IF_GROUP_NUM )
    {
        return MD_PARAM;
    }

    grp = md_ifgrp_get_copytogrp(grp_id);
    if ( grp == NULL )
    {
        return MD_MEMORY;
    }

    work_table = md_ifgrp_get_work_table_head();
    if (work_table == NULL)
    {
        return MD_INIT;
    }

    grp_work = &grp->grp_work;

    /* back up priority */
    for ( i = 0; i < MAX_GRP_ACL; i++ )
    {
        priority_backup[i] = grp_work->priority[i];
    }

    for ( i = 0; i < MAX_GRP_ACL; i++ )
    {
        grp_work->priority[i] = INVALID_IFGRP_WORK_INDEX;
        if ( priority_backup[i] == INVALID_IFGRP_WORK_INDEX)
        {
            continue;
        }
        if ( priority_backup[i] != i)
        {
            grp_work->priority[priority_backup[i]] = priority_backup[i] ;
        }
        else
        {
            grp_work->priority[i] = i ;
        }
    }


    /* sort */
    master = grp_work->master;
    backup = !master;
    for ( i = 0; i < MAX_GRP_ACL; i++)
    {
        grp_work->grp_acl[backup][i] = NULL;
    }

    for ( i = 0, j = 0; i < MAX_GRP_ACL; i++)
    {
        if ( grp_work->priority[i] == INVALID_IFGRP_WORK_INDEX)
        {
            continue;
        }
        grp_work->grp_acl[backup][j++] =
            work_table[grp_work->priority[i]].acl_callback;
    }

    /* modify master */
    grp_work->master  = backup;
    grp_work->mdu_acl = grp_work->grp_acl[backup];
    grp_work->md_acl  = MD_PTR_TO_PHYS(grp_work->grp_acl[backup]);
    for ( i = 0; i < MAX_GRP_ACL; i++)
    {
        grp_work->grp_acl[master][i] = grp_work->grp_acl[backup][i];
    }
    return MD_SUCCESS;
}


int md_update_oifgrp_up_port_array(int if_id, uint8_t link)
{
    mpp_epif_entry *epif = NULL;
    oif_grp        *grp  = NULL;
    int index, bit;
    uint8_t master, backup;
    int i, j, k;
    uint32_t up_prort_num;
    oif_grp_up_port_member *up_port_master = NULL, *up_port_backup = NULL;
    int weight;
    int rv;

    /* check parameter */
    if ( if_id >= MAX_IF_NUM )
    {
        return MD_PARAM;
    }

    epif = md_ifgrp_get_epif(if_id);
    if ( epif == NULL )
    {
        return MD_MEMORY;
    }

    for ( i = 0; i < IF_GROUP_NUM; i++ )
    {
        grp = md_ifgrp_get_oifgrp(i);
        if ( grp == NULL )
        {
            return MD_MEMORY;
        }
        index = if_id / UINT64_BIT_NUM;
        bit   = if_id % UINT64_BIT_NUM;
        if ( !(grp->ifmask[index] & (1 << (UINT64_BIT_NUM - bit -1))) )
        {
           continue;
        }
        if (link)
        { /* down to up */
            if ( grp->up_ifmask[index] & (1 << (UINT64_BIT_NUM - bit -1)) )
            {
                printf("%s %d error\n", __func__, __LINE__);
            }
            /* set interface flag */
            grp->up_ifmask[index] |= 1 << (UINT64_BIT_NUM - bit -1);
            master = grp->master;
            backup = !master;

            up_port_master = &grp->up_port[master];
            up_port_backup = &grp->up_port[backup];

            /* get interface weight */
            if (grp->mode == OIF_GRP_MODE_WEIGHT_ROUND_ROBIN ||
                    grp->mode == OIF_GRP_MODE_WEIGHT_HASH)
            {
                weight = 10;
            }
            else
            {
                weight = 1;
            }

            /* add interface to up port array */
            rv =  md_oifgrp_add_if_to_up_port_array(up_port_backup, if_id, weight);
            if (rv)
            {
                return rv;
            }

            grp->master = backup;
            /* copy current up port array to old master array */
            up_port_master->up_port_num = up_port_backup->up_port_num;
            for ( i = 0; i < up_port_master->up_port_num; i++)
            {
                up_port_master->up_port[i] = up_port_backup->up_port[i];
            }
        }
        else
        { /* up to down */
            if ( !(grp->ifmask[index] & (1 << (UINT64_BIT_NUM - bit -1))) )
            {
                printf("error\n");
            }
            /* clear up if_mask flag */
            grp->up_ifmask[index] &= ~((uint64_t)(1 << (UINT64_BIT_NUM - bit -1)));

            master = grp->master;
            backup = !master;

            up_port_master = &grp->up_port[master];
            up_port_backup = &grp->up_port[backup];

            /* get interface weight */
            if (grp->mode == OIF_GRP_MODE_WEIGHT_ROUND_ROBIN ||
                    grp->mode == OIF_GRP_MODE_WEIGHT_HASH)
            {
                weight = 10;
            }
            else
            {
                weight = 1;
            }

            /* del interface from up port array */
            rv = md_oifgrp_del_if_from_up_port_array(up_port_backup, if_id, weight);
            if (rv)
            {
                return rv;
            }

            grp->master = backup;

            /* copy current up port array to old master array */
            up_port_master->up_port_num = up_port_backup->up_port_num;
            for ( i = 0; i < up_port_master->up_port_num; i++)
            {
                up_port_master->up_port[i] = up_port_backup->up_port[i];
            }
        }
    }
    return MD_SUCCESS;
}
