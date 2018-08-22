#include "kstr_mq_ring.h"

/*
* 参数size 代表fifo的大小，必须为2的幂次方
*/
ring_t* initialize_ring(uint32_t size) 
{
   ring_t *r = calloc(1, sizeof(ring_t));
   r->idx_buff = calloc(size, sizeof(uint64_t));
   r->size     = size;
   r->in = r->out = 0;
   pthread_mutex_init(&(r->lock), NULL);
   return r;
}
 
bool enring(ring_t *r, kstr_pkt_t *value) 
{
   pthread_mutex_lock(&r->lock) ;        // Acquire t_lock in order to access Tail
   if (r->in - r->out == r->size)
   {
      pthread_mutex_unlock(&r->lock) ;   // Release t_lock      
      return false;
   }
   r->idx_buff[r->in & (r->size-1)] = (uint64_t *)value;
   ++r->in;
   pthread_mutex_unlock(&r->lock) ;      // Release t_lock
   return true;
}
 

bool dering(ring_t *r, kstr_pkt_t **pvalue) 
{
   if(r->in == r->out)
   {
      return false;
   }
   *pvalue = (kstr_pkt_t *)r->idx_buff[r->out & (r->size-1)];
   ++r->out; 
   return true;            // Queue was not empty, dequeue succeeded
}

void destory_ring( ring_t* r)
{ 
   int i;
   kstr_pkt_t *pkt=NULL;
   for (i = 0; i < r->size; ++i)
   {
      bool bet = dering(r,&pkt);
      if (bet)
      {
         free(pkt);
      }
      else
      {
         break;
      }
   }
   if (r->idx_buff)
   {
      free(r->idx_buff);
   }
   free(r);
}
	
