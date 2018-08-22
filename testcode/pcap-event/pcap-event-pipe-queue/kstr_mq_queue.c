#include "kstr_mq_queue.h"

queue_t* initialize_queue( ) {
   queue_t *q = calloc(1, sizeof(queue_t));
   node_t* node = calloc(1, sizeof(node_t));   // Allocate a free node
   node->next = NULL;   // Make it the only node in the linked list
   q->head = q->tail = node;   // Both head and tail point to it
   pthread_mutex_init(&(q->q_h_lock), NULL);
   pthread_mutex_init(&(q->q_t_lock), NULL);
   return q;
}
 
void enqueue(queue_t *q, kstr_str_t *value) {
   node_t* node = calloc(1, sizeof(node_t));       // Allocate a new node from the free list
   node->value = value ;    // Copy enqueued value into node
   node->next = NULL ;      // Set next pointer of node to NULL
   pthread_mutex_lock(&q->q_t_lock) ;     // Acquire t_lock in order to access Tail
   q->tail->next = node; // Link node at the end of the queue
   q->tail = node     ;  // Swing Tail to node
   q->count += 1;
   pthread_mutex_unlock(&q->q_t_lock) ;   // Release t_lock
}
 

bool dequeue(queue_t *q, kstr_str_t **pvalue) {
   pthread_mutex_lock(&q->q_h_lock);   // Acquire h_lock in order to access Head
   node_t*  node = q->head;    // Read Head
   node_t*  new_head = node->next;       // Read next pointer
   if(new_head == NULL)
   {         // Is queue empty?
      pthread_mutex_unlock(&q->q_h_lock);     // Release h_lock before return
      return false;             // Queue was empty
   }
   *pvalue = new_head->value;   // Queue not empty, read value
   q->head = new_head;  // Swing Head to next node
   q->count -= 1;
   pthread_mutex_unlock(&q->q_h_lock);   // Release h_lock
   free(node);             // Free node
   return true;            // Queue was not empty, dequeue succeeded
}

void destory_queue( queue_t* q )
{
   int i;
   int qlen = q->count;
   if(qlen == 0)
   {
	free(q->head);
	free(q);
   }
   for(i = 0; i < qlen; i++)
   {
	   if(q->count == 0)
   	{
        	free(q->head);
        	free(q);
	   }
   	kstr_str_t* pvalue = NULL;
   	dequeue( q, &pvalue );		
   	free( pvalue->ptr );
   	free( pvalue);
   }  
}
	
