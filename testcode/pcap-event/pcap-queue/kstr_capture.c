#include "kstr_capture.h"
#include "kstr_worker_thread.h"
#include "kstr_mq_queue.h"
#include "kstr_mq_ring.h"

event_thread_ctx_t  *   threads; 
int nthreads = 8;

typedef struct
{
    int    id;
    int    last_thread;
    #if USE_MSG_RING
    ring_t *msgr;
    #endif
}libpcap_param_t;

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
 	libpcap_param_t *tparam = (libpcap_param_t *)arg;
 	int last_thread = tparam->last_thread;  
  	/*printf("id: %d\n", ++(*id));  
  	printf("Packet length: %d\n", pkthdr->len);  
  	printf("Number of bytes: %d\n", pkthdr->caplen);  
  	printf("Recieved time: %s", ctime((const time_t *)&pkthdr->ts.tv_sec));   
	*/
  	if(pkthdr->caplen <10) return; 
  	if(pkthdr->caplen >2000) return; 

	int tid = ( last_thread + 1) % nthreads ;
	tparam->last_thread = tid;
	event_thread_ctx_t* local_thread = threads + tid;


#if USE_MSG_RING
    kstr_pkt_t * enc = NULL;
    bool bet = dering(tparam->msgr, &enc);
    if (bet)
    {
    	memset(enc, 0,sizeof(kstr_pkt_t));
    	enc->r   = tparam->msgr;
    	enc->len = 	pkthdr->caplen;
    	memcpy(enc->packet, packet, pkthdr->caplen);
    	bool bett = enring(local_thread->msgr, enc);
    	if (!bett)
    	{
            //printf("pcap enring worker failed\n");
    		printf("*");
    		bett = enring(tparam->msgr, enc);
    		if (!bett)
    		{
    			printf("pcap enring again failed\n");
    		}
    	}
    } 
#else
    if (write(local_thread->notify_send_fd, "s", 1) != 1) 
    {
        perror("Writing to thread notify pipe");
    }
   	kstr_str_t * enc = calloc(1, sizeof(kstr_str_t));
	u_char* pac = calloc( 1, pkthdr->len + 1 );
	memcpy(pac, packet, pkthdr->caplen);
	enc->ptr = pac;
	enc->len = pkthdr->caplen;       
	enqueue( local_thread->msgq, enc);
#endif
}

void * pkt_cap( void *arg )
{
	char  *devicename = (char*)arg;
	char serror[PCAP_ERRBUF_SIZE] = {0};
	libpcap_param_t tparam;
	memset(&tparam, 0, sizeof(libpcap_param_t));
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
#if USE_MSG_RING
        int i;
        tparam.msgr = initialize_ring(KSTR_PCAP_RING_SIZE);
        printf("tparam.msgr=%p\n", tparam.msgr);
        kstr_pkt_t *pkt = calloc(KSTR_PCAP_RING_SIZE, sizeof(kstr_pkt_t));
        if (pkt)
        {
	        for (i=0; i < KSTR_PCAP_RING_SIZE; i++)
	        {	
	        	bool bet = enring(tparam.msgr, pkt+i);
	        	if (!bet)
	        	{
	        		printf("have enring %d\n", i);
	        		break;
	        	}
	        }
	        printf("have enring %d\n", i);        	
        }
#endif
	pcap_loop(handle, -1, get_packet_callback, (void*)&tparam);
	return NULL;
}

	
void releaseNetCapture(void)
{
	pcap_close(handle);
}


