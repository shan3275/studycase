#ifndef __MD_WQE_INCLUDE__
#define __MD_WQE_INCLUDE__
#include "md_config.h"
#ifndef PLATFORM_X86
typedef struct
{

    /*****************************************************************
     *  WORD 0
     *   HW WRITE: the following 64 bits are filled by HW when a packet arrives
     */

    ¦   cvmx_wqe_word0_t word0;

    /*****************************************************************
     *  WORD 1
     *  HW WRITE: the following 64 bits are filled by HW when a packet arrives
     */

    ¦   cvmx_wqe_word1_t word1;
    /** 
     *  WORD 2
     *   HW WRITE: the following 64-bits are filled in by hardware when a packet arrives
     *   This indicates a variety of status and error conditions.
     */
    cvmx_pip_wqe_word2_t       word2;

    /** 
     *  Pointer to the first segment of the packet.
     */
    cvmx_buf_ptr_t             packet_ptr;

    /** 
     *  HW WRITE: octeon will fill in a programmable amount from the
     *            packet, up to (at most, but perhaps less) the amount
     *            needed to fill the work queue entry to 128 bytes
     *  If the packet is recognized to be IP, the hardware starts (except that
     *  the IPv4 header is padded for appropriate alignment) writing here where
     *  the IP header starts.
     *  If the packet is not recognized to be IP, the hardware starts writing
     *  the beginning of the packet here.
     */
    uint8_t packet_data[96];


    /**
     * If desired, SW can make the work Q entry any length. For the
     * purposes of discussion here, Assume 128B always, as this is all that
     * the hardware deals with.
     *
     */

}  CVMX_CACHE_LINE_ALIGNED cvmx_wqe_t;

/*
 *
 *  Intermedia control block, should limit to fit the
 *
 *  packet_data in wqe
 *
 */
struct mpp_control
{
    /* 0  */
    uint8_t *packet;

    /* 1 */
    uint8_t  ipif;   /* logic port number */
    uint8_t  len_header;  /* ip head offset */
    int16_t  rev0;
#define SSN_INDEX_INVALID 0xffffffff
    uint32_t ssn_index;

    /* 2 */
    struct mpp_ssn *ssn;

    /* 3 */
    struct udphdr *udph;

    /* 4 */
    uint8_t  session_type;
    uint8_t  vlan_act;            /* 3:5 */
#define     MPP_VLAN_DO_NOTHING   0xe0
#define     MPP_VLAN_ADD          0xc0
#define     MPP_VLAN_DEL          0xa0
#define     MPP_VLAN_TAG_MASK     0x1f
    uint8_t  mirrif;
    uint8_t  ilif;             /* ingress logical interface  */
    int32_t  dupl_num;       /* copy to times */

    /* 5 */
    /* for syncookie */
    uint32_t svr_seq;
    uint32_t syn_cookie;

    /* 6 */
    uint32_t  tag;
    int32_t  ssn_ip;

    /* 7 */
    struct mpp_tuple *tuple;

    /* 8 */
    union
    {
        struct gtphdr   *gtph;
        struct grehdr   *greh;
    };

    /* 10 : pkt_type */
    char pkt_type;
    uint32_t user_info_idx; /* 3gpp */

    /* 11 */
    union
    {
        uint32_t teid;   /* gn */
        uint32_t tlli;   /* gb */
        uint32_t grekey; /* cdma */
    };
};

#else

typedef struct
{
    uint8_t port;
    uint8_t packet_data[96];
} cvmx_wqe_t;

/*
 *
 *  Intermedia control block, should limit to fit the
 *
 *  packet_data in wqe
 *
 */
struct mpp_control
{
    /* 0  */
    uint8_t *packet;

    /* 1 */
    uint8_t  ipif;   /* logic port number */
    uint8_t  len_header;  /* ip head offset */
    int16_t  rev0;
#define SSN_INDEX_INVALID 0xffffffff
    uint32_t ssn_index;

    /* 2 : pkt_type */
    char pkt_type;
    uint32_t user_info_idx; /* 3gpp */
    uint8_t copyto_times;
    uint8_t action;

};
#endif /* end of PLATFORM_X86 */


#endif /* end of __MD_WQE_INCLUDE__ */
