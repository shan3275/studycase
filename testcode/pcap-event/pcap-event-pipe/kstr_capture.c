#include "kstr_capture.h"
#include "kstr_worker_thread.h"

event_thread_ctx_t  *   threads; 
int nthreads = 2;

void capture_init(int nthreads)
{
        threads =  worker_thread_init( nthreads );
}

void capture_destory(void)
{
 
	worker_thread_close(threads, nthreads );
}

static pcap_t* handle = NULL;
	
void get_packet_callback(u_char * arg, const struct pcap_pkthdr * pkthdr, const u_char * packet)  
{  
 	int *last_thread = (int *)arg;  
 
   
  	/*printf("id: %d\n", ++(*id));  
  	printf("Packet length: %d\n", pkthdr->len);  
  	printf("Number of bytes: %d\n", pkthdr->caplen);  
  	printf("Recieved time: %s", ctime((const time_t *)&pkthdr->ts.tv_sec));   
	*/
  	if(pkthdr->caplen <10) return; 

	int tid = ( (* last_thread) + 1) % nthreads ;
	*last_thread = tid;
	event_thread_ctx_t* local_thread = threads + tid;

	if (write(local_thread->notify_send_fd, packet, pkthdr->len) != pkthdr->len) {
               	perror("Writing to thread notify pipe");
       	}


}

void * pkt_cap( void *arg )
{
	int last_thread=0;
	char  *devicename = (char*)arg;
	char serror[PCAP_ERRBUF_SIZE] = {0};
	if(NULL == devicename)
	{
		//printf("--NetCapture.c // open_capture //  devicename get failed\n ");
		printf("--NetCapture.c // open_capture //  devicename get failed\n ");
		return NULL;
	}
		 
	handle = pcap_open_live(devicename, 65535, 1, 0, serror);
        if(NULL == handle)
        {
                printf("--NetCapture.c // newNetCapture // pcap_open_live  // failure : %s \n", serror);
                return NULL;
        }
	pcap_loop(handle, -1, get_packet_callback, (void*)&last_thread);
	return NULL;
}

	
void releaseNetCapture(void)
{
	pcap_close(handle);
}


