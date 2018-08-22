#include "init.h"
#include "yd_capture.h"
#include "yd_resource.h"

static pcap_t* handle = NULL;

	
void get_packet_callback(u_char * arg, const struct pcap_pkthdr * pkthdr, const u_char * packet)  
{  
 	sContext *ctx = (sContext *)arg;  
 
   
  	/*printf("id: %d\n", ++(*id));  
  	printf("Packet length: %d\n", pkthdr->len);  
  	printf("Number of bytes: %d\n", pkthdr->caplen);  
  	printf("Recieved time: %s", ctime((const time_t *)&pkthdr->ts.tv_sec));   
	*/
  	if(pkthdr->caplen <10) return; 


        //用接受的数据的时间戳来进行线程映射
	char* ttime = ctime((const time_t *)&pkthdr->ts.tv_sec) ;
        char identfy[128] = {0};
	sprintf(identfy, "%s%d", ttime, pkthdr->len);

	uint32_t idx = 0;
	ecr_crc32_hash_mix( identfy, strlen(identfy), (u_int32_t) time(NULL), &idx) ;

	int tid = idx % ctx->conf_ctx_t->app_threadnums ;

	event_thread_ctx_t* local_thread = ctx->threads + tid;

	//u_char* sendbuf = calloc( 1, pkthdr->len);
	//memcpy( sendbuf, packet, pkthdr->len );

	if (write(local_thread->notify_send_fd, packet, pkthdr->len) != pkthdr->len) {
               	perror("Writing to thread notify pipe");
       	}


} 



void* pkt_cap( void *arg )
{
	sContext *ctx = (sContext*)arg;
	        //采集网络报文
       	char* devicename = ctx->conf_ctx_t->env_capture_device;
	char serror[PCAP_ERRBUF_SIZE] = {0};
	if(NULL == devicename)
	{
		//printf("--NetCapture.c // open_capture //  devicename get failed\n ");
		printf("--NetCapture.c // open_capture //  devicename get failed\n ");
		return -1;

	}
		 
	handle = pcap_open_live(devicename, 65535, 1, 0, serror);
        if(NULL == handle)
        {
                printf("--NetCapture.c // newNetCapture // pcap_open_live  // failure : %s \n", serror);
                return -2;
        }
	pcap_loop(handle, -1, get_packet_callback, (void*)ctx);
}

	
void releaseNetCapture()
{
	pcap_close(handle);
}


