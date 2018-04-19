#include "dc_global.h"
#include "zmq_help.h"
#include <sys/timeb.h>

#define MAX_IP_LEN 32
#define WIN_SENDER_LEN 8
#define WIN_SENDER_SUB_LEN 2
#define TOTAL_IP_SUM 250000
#define TEST

extern char g_currTime[16][64];
static void* g_pvHashWinSender[MAX_NUM_THREADS] = { NULL };
//extern unsigned long hash_map_get_hash_number(char *start, int strlen, int hashLen);
extern char** common_get_range_addr(const char * addr_begin, int num);
extern HashMap* hash_map_new(unsigned int length);
extern int hash_map_insert(HashMap* psHashMap, char*start, int len, long* in_val);
extern int hash_map_find(HashMap* psHashMap, char*start, int len, long* out_val);

static pthread_mutex_t zmq_send_lock;
zmq_pollitem_t g_items [MAX_NUM_THREADS] ;

/*存储所有的IP信息*/
HashMap* g_pTotalIP[THREAD_NUM] = {NULL};

/************************************************************************/
//  [8/6/2014 fushengli]                                               
//  初始化zmq context                                                                   
/************************************************************************/
void mq_pop_win_sender_pop_init()
{
	int i = 0;
	void *client[MAX_NUM_THREADS] = {NULL};
	for (; i < MAX_NUM_THREADS; i++)
	{
		zctx_t *ctx = zctx_new ();
		client[i] = zsocket_new (ctx, ZMQ_DEALER);
		char identity [10];
		sprintf (identity, "%s", g_conf_pop_client_identify);
		zsocket_set_identity (client[i], identity);
		zsocket_connect (client[i], g_conf_pop_client_adr);
		g_items[i].socket = client[i];
		g_items[i].events = ZMQ_POLLOUT;
	}

	for (i = 0; i < MAX_NUM_THREADS; i++)
	{
		g_pTotalIP[i] = hash_map_new(TOTAL_IP_SUM);
	}
	return;
}





void pop_mq_direct_send_msg(const int tid, const struct pfring_pkthdr * hdr,  const char* UA, const char* host, const char* key_word, const char* ts)
{
    	char jsonstr[1024*5] = {'\0'};
	u_int32_t ip = 0;
	if(hdr->extended_hdr.parsed_pkt.tunnel.tunnel_id != NO_TUNNEL_ID)
	{
		ip = hdr->extended_hdr.parsed_pkt.tunnel.tunneled_ip_src.v4;
	}
	else
	{
		ip = hdr->extended_hdr.parsed_pkt.ipv4_src;
	}
		

	if (0 != ip && strlen(UA) != 0)
	{
		char ipstr[MAX_IP_LEN];
		inet_ntoa_r(ip, ipstr);

    		int iSendNum = hash_map_get_hash_number(ipstr, strlen(ipstr), MAX_NUM_THREADS);

		sprintf(jsonstr, "{\"ip\":\"%s\", \"UA\":\"%s\", \"host\":\"%s\", \"key_word\":\"%s\", \"ts\":\"%s\"}", ipstr, UA, host, key_word, ts);	
		//printf("%s\n", jsonstr);
		zmq_poll(g_items, MAX_NUM_THREADS, 10 * ZMQ_POLL_MSEC);
		zframe_t * frame = zframe_new(jsonstr, strlen(jsonstr));
		int i = 0;
		for (; i<MAX_NUM_THREADS; i++)
		{
			if (g_items [i].revents & ZMQ_POLLOUT) {
				pthread_mutex_lock(&zmq_send_lock);
				zframe_send(&frame, g_items[i].socket, ZFRAME_REUSE);
				pthread_mutex_unlock(&zmq_send_lock);
				break;
			}
		}
		//s_send (g_pvHashWinSender[iSendNum], jsonstr);
		zframe_destroy(&frame);
	}

}
