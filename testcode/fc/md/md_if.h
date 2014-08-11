#include "md_config.h"

#ifndef __MD_IF_INCLUDE__
#define __MD_IF_INCLUDE__

#define MAX_CORE_NUM 32
#define MPP_MAX_IF_NUM 40

typedef struct mpp_if_stat_t
{
    union
    {
        struct
        {
            int64_t         rx_pkts;        /* the number of received pakcets now */
            int64_t         rx_bytes;       /* the size of received pakcets in bytes now*/
            int64_t         tx_pkts;
            int64_t         tx_bytes;
            uint64_t        rx_pps;         /* the number of received pakets per second */
            uint64_t        tx_pps;
            uint64_t        rx_max_pps;
            uint64_t        tx_max_pps;
            uint64_t        rx_bps;         /* the size of received packets in bits per second */
            uint64_t        rx_max_bps;
            uint64_t        tx_bps;
            uint64_t        tx_max_bps;
            uint64_t        rx_bytes_pre;
            uint64_t        rx_pkts_pre;
            uint64_t        tx_bytes_pre;
            uint64_t        tx_pkts_pre;
        };
        uint64_t        data_64[16];
    };
} mpp_if_stat;

#define SIZE_OF_EPIF_ENRTY 516
typedef struct mpp_epif_entry_t
{
    union
    {
        struct
        {
            /* word 0 */
            uint64_t     online                     :1;   /* online flag*/
            uint64_t     port_num                   :8;   /* logical interface number */
            uint64_t     slot                       :4;   /* slot number , like slot/card/index */
            uint64_t     card                       :2;   /* card number, like slot/card/index */
            uint64_t     index                      :5;   /* index number, like slot/card/index */  
            uint64_t     ipd_port                   :16;  /* packet port */
            uint64_t     if_type                    :2;   /* interface physical attribute, such as lan, wan*/
            uint64_t     mode                       :3;   /* interface mode,such as inline, inline_server, sniffer,user integrality and so on*/
            uint64_t     speed                      :3;   /* interface speed */
            uint64_t     mtu                        :16;  
            uint64_t     link                       :1;   /* interface link status, 1 for link up, 0 for link down */
            uint64_t     flag_singlink              :1;   /* 1 for link-forced, also called single fiber mode */
            uint64_t     shutdown                   :1;   /* interface shutdown, 1 for shutdown; 0 for not shutdown */ 
            uint64_t     monitor_flag               :1;   /* interface status loop enable flag */

            /* word 1 */
            uint64_t     flag_time_stamp            :1;   /* time stamp enable flag, write time stamp to dmac */
            uint64_t     flag_mod_dmac              :1;   /* modify dmac enable flag */
            uint64_t     flag_dmac_mask             :1;   /* modify dmac mask enable flag */
            uint64_t     mac_mask                   :16;  
            uint64_t     flag_big_sync              :1;   /* flag that length of tcp sync header >= 32bytes */
            uint64_t     flag_only_sync             :1;   /* tcp sync packet statistics enable flag */
            uint64_t     qos_en                     :1;   /* reserved */
            uint64_t     qos_id                     :16;  /* reserved */
            uint64_t     weight                     :8;  
            uint64_t     iifgrp                     :8;   /*interface bind to input interface group */
            uint64_t     load_flag                  :2;   /* interface upload or download flag */
            uint64_t     backepif                   :8;   /* backup interface */    
            //uint8_t      cifgrp[8];                      /* copy to interface group */

                /* word 2 */
            uint64_t     vlan                       :16;  /*default vlan */ 
            uint64_t     hijacked                   :7; 
            uint64_t     auditid                    :4;  /* Maxinum 16 audit group(s) */ 
            uint64_t     flag_pppoe_with_vlan       :1; 
            uint64_t     flag_clear_mpls            :1;  
            uint64_t     flag_clear_vlan            :1; 
            uint64_t     flag_3gpp_gn               :1; 
            uint64_t     flag_3gpp_gb               :1;

                /* word 3 */
            uint64_t     tx_en                      :1;   /* flag tx statistics enable/disable */
            uint64_t     rx_en                      :1;   /* flag rx statistics enable/disable */
            uint64_t     ifname_index               :3;   /* interface name index */
            uint64_t     board_type_index           :3;   /* board type index */
            uint64_t     reserved0                  :56;  /* reserved */

                /* word 4---word 515 */
            mpp_if_stat   if_stat[MAX_CORE_NUM];    /* interface statistics */

        };

        uint64_t    data[SIZE_OF_EPIF_ENRTY];
    };
#ifndef PLATFORM_X86
} CVMX_CACHE_LINE_ALIGNED mpp_epif_entry;
#else
}  mpp_epif_entry;
#endif

typedef struct mpp_epif_t
{
    mpp_epif_entry    epif_data[MPP_MAX_IF_NUM];
#ifndef PLATFORM_X86
} CVMX_CACHE_LINE_ALIGNED mpp_epif;
#else
} mpp_epif;
#endif

int md_if_init(void);
int if_get_if_online( uint8_t ipif, uint8_t *online);
int if_set_if_online ( uint8_t ipif, uint8_t online);
int if_get_if_link( uint8_t ipif, uint8_t *link);
int if_set_if_link ( uint8_t ipif, uint8_t flag);
int if_get_if_iifgrp( uint8_t ipif, uint8_t *ifgrp);
int if_set_if_iifgrp ( uint8_t ipif, uint8_t ifgrp);
int if_get_if_weight( uint8_t ipif, uint8_t *weight);
int if_set_if_weight ( uint8_t ipif, uint8_t weight);
int if_get_if_phy( uint8_t ipif, uint8_t *link);
int if_set_if_phy( uint8_t ipif, uint8_t link);
void md_if_status_loop(void);
#endif /* end of __MD_IF_INCLUDE__ */
