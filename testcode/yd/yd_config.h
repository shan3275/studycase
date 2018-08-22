#ifndef YDCAP_CONFIG
#define YDCAP_CONFIG

#include "confile.h"

struct yd_config_ctx_s{

        int           app_threadnums;
        int           app_threadidx;
        u_char*       app_env;

        u_char*       resource_redis_hostname;
        int           resource_redis_port;

        u_char*       resource_mysql_host;
        int           resource_mysql_port;
        u_char*       resource_mysql_user;
        u_char*       resource_mysql_passwd;
        u_char*       resource_mysql_db;
        u_char*       resource_mysql_charset;

	int           resource_log_type;
	u_char*       resource_log_path;

        u_char*       env_capture_device;
};

typedef struct yd_config_ctx_s yd_config_ctx_t;

yd_config_ctx_t*   yd_config_init( const char* filename );

void yd_config_destory( yd_config_ctx_t* config_ctx_t ) ;

#endif
