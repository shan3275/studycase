#include "init.h"
#include "yd_capture.h"
#include "yd_resource.h"




int main(int argc, char* argv[])
{

	//初始化上下文
	sContext* ctx =  context_init();
	if(NULL == ctx)
		exit(-1);

	//开一个线程进行抓包
        pthread_t capThread;
        int ret = pthread_create(&capThread, NULL, pkt_cap, (void*)ctx);
        if (ret != 0)  
        {  
		dzlog_error("Create capThread error!\n");
                //printf("Create capThread error!\n");
                return -1;
        }
	
	pthread_join(capThread, NULL);

	context_destory( ctx );
	return 0;
}
