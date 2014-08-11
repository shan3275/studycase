#include "md_config.h"
#include "md_if.h"
#include "md_wqe.h"
#ifndef __MD_GRP_INCLUDE__
#define __MD_GRP_INCLUDE__
#define MAX_GRP_ACL 10

#ifdef __i386__
#define UINT64_BIT_NUM 32
typedef uint32_t  IF_MASK_T;
#else
typedef uint64_t  IF_MASK_T;
#define UINT64_BIT_NUM 64
#endif

#define MAX_IF_NUM  40
#define IF_GROUP_NUM  40
#define INVALID_IIFGRP  0xFF
#define INVALID_OIFGRP  0xFF
#define INVALID_IFGRP_WORK_PRIORITY  0xFF
#define INVALID_IFGRP_WORK_INDEX     0xFF

#if (MAX_IF_NUM % UINT64_BIT_NUM)
#define IF_MASK_NUM (MAX_IF_NUM / UINT64_BIT_NUM + 1)
#else
#define IF_MASK_NUM (MAX_IF_NUM / UINT64_BIT_NUM )
#endif

typedef int  (* if_grp_acl_callback) ( cvmx_wqe_t *work,  struct mpp_control *mpp );

typedef enum
{
    MD_IF_GRP_WORK_ACL,
    MD_IF_GRP_WORK_IP,
    MD_IF_GRP_WORK_STRING,
    MD_IF_GRP_WORK_PAYLOAD,
    MD_IF_GRP_WORK_MAX,
    MD_IF_GRP_WORK_INVALID,
}md_ifgrp_work_t;

typedef struct if_grp_member_t
{
    uint32_t                if_grp_port_num;    /* port number in a interface group */
    IF_MASK_T                ifmask[IF_MASK_NUM];   /* interface mask*/
}if_grp_member;

typedef struct if_grp_work_t
{
    if_grp_acl_callback     *mdu_acl;       /* mdu operate acl priority pointer, application address */
    if_grp_acl_callback     *md_acl;        /* md operate acl priority pointer, physical address */
    uint32_t                master;         /* indicate which acl table in using */
    if_grp_acl_callback     grp_acl[2][MAX_GRP_ACL];     /* two group, one for use,  the other for backup */
    uint8_t                 priority[MAX_GRP_ACL];       /* acl work default priority */
}if_grp_work;


typedef struct iif_grp_t
{
    uint32_t                if_grp_id;      /* interface group index */
    if_grp_member           grp_member;     /* interface group member */
    if_grp_work             grp_work;       /* interface group work */
}iif_grp;

#define MAX_COPY_TO_TIMES  5
typedef struct copyto_grp_t
{
    uint32_t                if_grp_id;      /* interface group index */
    if_grp_work             grp_work;       /* interface group work */
}copyto_grp;

typedef enum oif_grp_mode_t
{
    OIF_GRP_MODE_ROUND_ROBIN,
    OIF_GRP_MODE_WEIGHT_ROUND_ROBIN,
    OIF_GRP_MODE_HASH,
    OIF_GRP_MODE_WEIGHT_HASH,
    OIF_GRP_MODE_MAX,
}oif_grp_mode;

typedef enum oif_grp_hash_t
{
    OIF_GRP_HASH_SIP,
    OIF_GRP_HASH_DIP,
    OIF_GRP_HASH_USER_INTEGRITY,
    OIF_GRP_HASH_SDIP,
    OIF_GRP_HASH_FIVE_TUPLE,
    OIF_GRP_HASH_MAX,
}oif_grp_hash;

#define OIF_GRP_UP_PORT_MEMBER_MAX  ( MAX_IF_NUM *10 )
typedef struct oif_grp_up_port_member_t
{
    uint32_t                up_port_num;  /* link up port number, add the weight, port number of up_port[] array */
    uint32_t                up_port[OIF_GRP_UP_PORT_MEMBER_MAX]; /* link up port logic number */
}oif_grp_up_port_member;

typedef struct oif_grp_t
{
    uint32_t                if_grp_id;           /* interface group index */
    uint32_t                port_num;            /* port number in a interface group */
    uint32_t                dn_port_num;         /* link down port number */
    IF_MASK_T               ifmask[IF_MASK_NUM];      /* interface mask  */
    IF_MASK_T               up_ifmask[IF_MASK_NUM];   /* up interface mask    */
    uint8_t                 mode;
    uint8_t                 hash;
    uint8_t                 master;       /* indicate which up port is using */
    oif_grp_up_port_member  up_port[2];   /* up port array, used for tarnsmit the packet */
}oif_grp;

typedef struct if_grp_work_table_t
{
    uint8_t priority;   /* default priority */
    if_grp_acl_callback  acl_callback;
}if_grp_work_table;

typedef struct mpp_if_group_t
{
    iif_grp             iif_group[IF_GROUP_NUM];
    oif_grp             oif_group[IF_GROUP_NUM];
    copyto_grp          copyto_group[IF_GROUP_NUM];
    if_grp_work_table   work_table[MD_IF_GRP_WORK_MAX];
}mpp_if_group;

int mpp_ifgrp_init(void);
int md_if_grp_search_work_table(if_grp_acl_callback acl_callback, uint8_t *index);
int md_ifgrp_work_register(md_ifgrp_work_t work, if_grp_acl_callback acl_callback);

int md_iifgrp_add(uint8_t grp_id, uint8_t if_id);
int md_iifgrp_del(uint8_t grp_id, uint8_t if_id);
int md_iifgrp_get(uint8_t grp_id, iif_grp *grp);
int md_iifgrp_bind_work(uint8_t grp_id, md_ifgrp_work_t work, int priority);
int md_iifgrp_unbind_work(uint8_t grp_id, md_ifgrp_work_t work);
int md_iifgrp_retore_work(uint8_t grp_id);

int md_oifgrp_add(uint8_t grp_id, uint8_t if_id);
int md_oifgrp_del(uint8_t grp_id, uint8_t if_id);
int md_oifgrp_get(uint8_t grp_id, oif_grp *grp);
int md_oifgrp_set_mode(uint8_t grp_id, oif_grp_mode mode);
int md_oifgrp_set_hash(uint8_t grp_id, oif_grp_hash hash);

int md_cifgrp_get(uint8_t grp_id, copyto_grp *grp);
int md_cifgrp_bind_work(uint8_t grp_id, md_ifgrp_work_t work, int priority);
int md_cifgrp_unbind_work(uint8_t grp_id, md_ifgrp_work_t work);
int md_cifgrp_retore_work(uint8_t grp_id);
int md_update_oifgrp_up_port_array(int if_id, uint8_t link);
#endif /* end of __MD_GRP_INCLUDE__ */
