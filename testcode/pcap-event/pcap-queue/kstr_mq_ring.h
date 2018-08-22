#ifndef _MQ_RING_
#define _MQ_RING_
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdint.h>

typedef struct ring_s {
    uint64_t *idx_buff;     /* the buffer holding the data pointer */
    uint32_t size;         /* the size of the allocated buffer */
    uint32_t in;           /* data is added at offset (in % size) */
    uint32_t out;          /* data is extracted from off. (out % size) */
    pthread_mutex_t lock; 
}ring_t;

#define KSTR_PKT_STR_LEN  2048
#define KSTR_PACKET_BUFF_LEN   2036
typedef struct kstr_pkt_s{
		ring_t   *r;
	    uint32_t len;
	    u_char   packet[KSTR_PACKET_BUFF_LEN];
}kstr_pkt_t;

#define KSTR_PCAP_RING_SIZE  1024   /* 必须是2的幂次方 */
#define KSRT_WORKER_RING_SIZE 128   /* 必须是2的幂次方 */

ring_t* initialize_ring(uint32_t size);
bool enring(ring_t *r, kstr_pkt_t* value);
bool dering(ring_t *r, kstr_pkt_t** pvalue) ;
void destory_ring( ring_t* r ) ;

#endif