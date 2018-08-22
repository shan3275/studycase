#ifndef _MQ_QUEUE_
#define _MQ_QUEUE_
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>

struct kstr_str_s{
        u_char* ptr;
        size_t len;
};

typedef struct kstr_str_s  kstr_str_t ;

typedef struct node_s {
    kstr_str_t* value; 
    struct node_s *next;
} node_t;
 
typedef struct queue_s {
    node_t *head; 
    node_t *tail;
    size_t count;
    pthread_mutex_t q_h_lock;
    pthread_mutex_t q_t_lock;
} queue_t;
 
queue_t* initialize_queue( ) ;
void enqueue(queue_t *q, kstr_str_t* value) ;
bool dequeue(queue_t *q, kstr_str_t** pvalue) ;
void destory_queue( queue_t* q ) ;
#endif