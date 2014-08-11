#include "md_embrave.h"
#include "md_init.h"
#include "md_type.h"
#include <time.h>

extern struct mpp_shared_data *gsdata;
int md_enabrave_init(void)
{
    gsdata->input_num = 0;
    return MD_SUCCESS;
}

int md_embrave_start(uint8_t num)
{
    if (num == 0 || num > MAX_EMBRAVE_NUM)
    {
        return MD_PARAM;
    }

    gsdata->input_num = num;

    return MD_SUCCESS;
}

void InitQueue(queue* pQ) 
{
    pQ->in  = 0;
    pQ->out = 0;
    pQ->count = 0;
}

void InQueue(queue* pQ, cvmx_wqe_t * cInData)
{
    if (pQ->count==MaxSize)
    {
        printf("\nQueue is overflow!\n");
        return;
    }

    pQ->cBuff[pQ->in++] =  cInData;
    pQ->in %= MaxSize;
    pQ->count++;
}

cvmx_wqe_t * OutQueue(queue* pQ)
{
    cvmx_wqe_t * ret = NULL;

    if (pQ->count==0)
    {
        //printf("\nqueue is blank\n");
        return ret;
    }

    pQ->count--;
    ret = pQ->cBuff[pQ->out];
    pQ->out++;
    pQ->out %= MaxSize;
    return(ret);
}

cvmx_wqe_t * cvmx_pow_work_request_sync(void)
{
    int i;
    cvmx_wqe_t *unit = NULL;
    if ( gsdata->input_num > 0)
    {
        for ( i = 0; i < gsdata->input_num; i++ )
        {
            /* generate packet to fill into fifo */
            unit = (cvmx_wqe_t *) malloc(sizeof(cvmx_wqe_t));
            memset((void *) unit, 0, sizeof(cvmx_wqe_t));
            srand( (unsigned)time( NULL ) );
            //unit->port = rand() % MAX_IF_NUM;
            unit->port = 3;
            printf("port:%d\n", unit->port);
            InQueue(&gsdata->pQ, unit);
        }
        gsdata->input_num = 0;
    }

    OutQueue(&gsdata->pQ);
}

void mpp_free_wqe_packet_data ( cvmx_wqe_t *work )
{
    if (work)
    {
        free((void *) work);
    }
}
