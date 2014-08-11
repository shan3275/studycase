#include "md_wqe.h"
#ifndef __MD_EMBRAVE_INCLUDE__
#define __MD_EMBRAVE_INCLUDE__

#define MAX_EMBRAVE_NUM 100
#define MaxSize  100
typedef struct
{
    cvmx_wqe_t *cBuff[MaxSize];
    int  in,out,count;
}queue;

int md_enabrave_init(void);
int md_embrave_start(uint8_t num);
cvmx_wqe_t * cvmx_pow_work_request_sync(void);
void mpp_free_wqe_packet_data ( cvmx_wqe_t *work );
#endif /* end of __MD_EMBRAVE_INCLUDE_ */
