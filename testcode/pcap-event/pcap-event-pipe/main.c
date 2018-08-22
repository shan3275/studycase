
#include "kstr_capture.h"

int main(int argc, char* argv[])
{
	char *device = "eth0";
	//初始化上下文
	capture_init(2);

	//开一个线程进行抓包
    pthread_t capThread;
    int ret = pthread_create(&capThread, NULL, pkt_cap, (void*)device);
    if (ret != 0)  
    {
            printf("Create capThread error!\n");
            return -1;
    }
	
	pthread_join(capThread, NULL);

	capture_destory();
	return 0;
}
