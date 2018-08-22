#include "yd_config.h"

static int conf_read( char* filename , yd_config_ctx_t* config_ctx_t  )
{

        INI_CONFIG* conf_ctx = ini_config_create_from_file( filename, 1 );
        if(NULL == conf_ctx)
		return -1;

        u_char* app_env = ini_config_get_string( conf_ctx, "common", "application.env", "develop");
        config_ctx_t->app_env = strdup( app_env );
        config_ctx_t->app_threadnums = ini_config_get_int( conf_ctx, "common", "application.worker.thread.nums", 5);
        config_ctx_t->app_threadidx = ini_config_get_int( conf_ctx, "common", "application.worker.thread.idx", 0);

	//下面是获取资源的部分
        u_char* resource_redis_hostname = ini_config_get_string( conf_ctx, "common:resource", "resource.redis.hostname", "127.0.0.1");
        config_ctx_t->resource_redis_hostname = strdup( resource_redis_hostname );

        config_ctx_t->resource_redis_port = ini_config_get_int( conf_ctx, "common:resource", "resource.redis.port", 6381);       


        config_ctx_t->resource_log_type = ini_config_get_int( conf_ctx, "common:resource", "log.type", 1);       
        u_char* resource_log_path = ini_config_get_string( conf_ctx, "common:resource", "log.confpath", "../conf/zlog.conf");
        config_ctx_t->resource_log_path = strdup( resource_log_path );

        if(strcmp( config_ctx_t->app_env, "product" ) == 0)
        {

                u_char* env_capture_device = ini_config_get_string( conf_ctx, "product:common", "capture.device", "eth0");
                config_ctx_t->env_capture_device = strdup( env_capture_device );

        }
        else
        {
                u_char* env_capture_device = ini_config_get_string( conf_ctx, "develop:common", "capture.device", "eth0");
                config_ctx_t->env_capture_device = strdup( env_capture_device );

        }
        printf("common \n");
        printf("\tapplication.io_thread : %d \n", config_ctx_t->app_threadnums);
        printf("\tapplication.app_env : %s \n", config_ctx_t->app_env);
        printf("\tresource.log.path : %s \n", config_ctx_t->resource_log_path);
        printf("\tresource.log.type : %d \n", config_ctx_t->resource_log_type);

        printf("%s : common\n", config_ctx_t->app_env);
        printf("\tcapture.device : %s \n", config_ctx_t->env_capture_device);


        ini_config_destroy( conf_ctx );
	return 0;
}


yd_config_ctx_t*  yd_config_init( const char* filename  )
{
        if(NULL == filename) return NULL;
        struct stat s;
        if(stat(filename,&s)) return NULL;

        yd_config_ctx_t* config_ctx_t = calloc(1, sizeof( struct yd_config_ctx_s ));
        conf_read( filename , config_ctx_t );

        return config_ctx_t;
}

void yd_config_destory( yd_config_ctx_t* config_ctx_t )
{
        if( NULL != config_ctx_t )
        {
                if( config_ctx_t->app_env != NULL ) free( config_ctx_t->app_env );
                if( config_ctx_t->resource_redis_hostname != NULL ) free( config_ctx_t->resource_redis_hostname );
                if( config_ctx_t->resource_log_path != NULL ) free( config_ctx_t->resource_log_path );
                if( config_ctx_t->env_capture_device != NULL ) free( config_ctx_t->env_capture_device );
                free( config_ctx_t );
        }
}

