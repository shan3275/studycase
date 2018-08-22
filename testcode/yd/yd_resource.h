#ifndef __SCONTEXT_H__
#define __SCONTEXT_H__

#include "init.h"
#include "yd_config.h"
#include "yd_worker_thread.h"

//资源统一管理   
typedef struct {
        yd_config_ctx_t     *   conf_ctx_t;       //配置文件资源
        void                *   zmq_ctx_t;        //zmq上下文
        zlog_category_t     *   recvlog_ctx_t;    //接收日志
        zlog_category_t     *   errorlog_ctx_t;   //错误日志
        event_thread_ctx_t  *   threads;          //线程池
}sContext;

//统一控制资源
sContext* context_init();
//
void context_destory( sContext* ctx);

#endif
