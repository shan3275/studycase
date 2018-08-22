#include "yd_resource.h"

#define CONFPATH "../conf/main.conf"

sContext* context_init(  )
{
	        sContext* ctx = calloc(1, sizeof( sContext ));

        //ctx->zmq_ctx_t = zmq_ctx_new();

        ctx->conf_ctx_t = yd_config_init( CONFPATH );
        if(ctx->conf_ctx_t == NULL)
        {
                printf("conf file init failed\n");
                return NULL;
        }

        int rc = zlog_init( ctx->conf_ctx_t->resource_log_path ) ;
        if(rc)
        {
                printf("init zlog failed\n");
                return NULL;
        }

        ctx->recvlog_ctx_t = zlog_get_category("YDServer_log");
        if (!ctx->recvlog_ctx_t) {
                printf("recvlog init fail\n");
                zlog_fini();
                return NULL;
        }

         ctx->errorlog_ctx_t = zlog_get_category("YDServer_log_error");
         if (!ctx->errorlog_ctx_t) {
                 printf("error log init fail\n");
                 zlog_fini();
                 return NULL;
         }

        ctx->threads =  worker_thread_init( ctx->conf_ctx_t->app_threadnums );
        return ctx;

}

void context_destory(  sContext * ctx )
{
        zlog_fini();
        yd_config_destory( ctx->conf_ctx_t );
        zmq_ctx_destroy( ctx->zmq_ctx_t );
	worker_thread_close( ctx->threads, ctx->conf_ctx_t->app_threadnums );
        free(ctx);
}

