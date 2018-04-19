#include "keywordctrl.h"
#include "util.h"
#include "czmq.h"
#include "log.h"
#include <sys/time.h>

#define MAX_THREAD_NUM 5

SKeyWordCtrl* newKeyWordCtrl(SKeyWordFilter* psKeyWordFilter, SToolRedisUtil* psRedisUtil)
{
	if(NULL == psKeyWordFilter)
	{
		printf("key word filter pointer is NULL\n");
		return NULL;
	}
	SKeyWordCtrl* psKeyWordCtrl = (SKeyWordCtrl*)malloc(sizeof(SKeyWordCtrl));
	memset(psKeyWordCtrl, 0, sizeof(SKeyWordCtrl));
	psKeyWordCtrl->initZmqRouter = initZmqRouter;
	//psKeyWordCtrl->getKeyWordDspId = getKeyWordDspId;

	psKeyWordCtrl->m_psKeyWordFilter = psKeyWordFilter;
	psKeyWordCtrl->m_psRedisUtil = psRedisUtil;

	return psKeyWordCtrl;
}

static void disposeData(void *args, zctx_t *ctx, void *pipe)
{
	SKeyWordCtrl* psKeyWordCtrl = (SKeyWordCtrl*) args;

	struct timeval start, end;
	void *worker = zsocket_new (ctx, ZMQ_DEALER);
	void *dsp_sender = zsocket_new (ctx, ZMQ_PUSH);
	zsocket_connect (worker, "inproc://backend");
	zsocket_connect (dsp_sender, psKeyWordCtrl->m_psKeyWordFilter->m_psCfgFileLoader->getDspInfo(psKeyWordCtrl->m_psKeyWordFilter->m_psCfgFileLoader, "popsearch"));
	printf("%s\n", psKeyWordCtrl->m_psKeyWordFilter->m_psCfgFileLoader->getDspInfo(psKeyWordCtrl->m_psKeyWordFilter->m_psCfgFileLoader, "popsearch"));

	printf("begin business ....\n");

	int i = 0;
	while (true) 
	{
		zmsg_t *msg = zmsg_recv (worker);
		if(!msg)
			break;
		zframe_t *identity = zmsg_pop (msg);
		zframe_t *content = zmsg_pop (msg);
		byte *data = zframe_data (identity);
		byte *con = zframe_data (content);
		zmsg_destroy (&msg);
		//printf("%s - %s",data, con);
		//log4c_category_log(mycat, LOG4C_PRIORITY_INFO, "%s - %s", data, con); 
		//发送给dsp平台

		//parser spark 
		MQ_Msg message = {{'\0'}, {'\0'},{'\0'}, {'\0'}, {'\0'}, {'\0'}};
		gettimeofday( &start, NULL );
		int ret = mq_msg_parser( con, &message);
		gettimeofday( &end, NULL );
		int timeuse = 1000000 * ( end.tv_sec - start.tv_sec ) + end.tv_usec -start.tv_usec;
		//LOG("time: %d us\n", timeuse);
		if(ret < 0)
		{
			continue;
		}
		printf("ip : %s, ad : %s, ua : %s, host : %s, key_word : %s, ts : %s\n", message.ip, message.ad, message.UA, message.host, message.key_word, message.ts);
		SAdvContent *psDspContent = (SAdvContent*)malloc(sizeof(SAdvContent));
		memset(psDspContent, 0 ,sizeof(SAdvContent));	
		ret = psKeyWordCtrl->m_psKeyWordFilter->getKeyWordDspId(psKeyWordCtrl->m_psKeyWordFilter, message.key_word, psDspContent);
		if(ret < 0)
		{
			continue;
		}
		char szADUA[2048] = {0};
		strcpy(szADUA, message.ad);
		strcat(szADUA, "::");
		strcat(szADUA, message.UA);

		char szADUAs[2048] = {0};
		strcpy(szADUAs, message.ad);
		strcat(szADUAs, message.UA);
		
		char cUaMD5s[33] = {'\0'};
        	rule_pop_encrypt_key_md5(szADUAs, cUaMD5s);
		int j = 0;
		for (j = 0; j < strlen(cUaMD5s); j++) cUaMD5s[j] = toupper(cUaMD5s[j]);
		zmq_send(dsp_sender, szADUA, strlen(szADUA), 1);
		//store szua + adlist to redis
		zlist_t * adlist = psDspContent->adlist;
		psKeyWordCtrl->m_psRedisUtil->saddToolRedisStr(psKeyWordCtrl->m_psRedisUtil, cUaMD5s, adlist);

		zframe_destroy(&identity);
		zframe_destroy(&content);
	}
}


void initZmqRouter(SKeyWordCtrl* psKeyWordCtrl)
{
	zctx_t *ctx = zctx_new ();
	void *frontend = zsocket_new (ctx, ZMQ_ROUTER);
	zsocket_bind (frontend, psKeyWordCtrl->m_psKeyWordFilter->m_psCfgFileLoader->getServerAdr(psKeyWordCtrl->m_psKeyWordFilter->m_psCfgFileLoader));

	void *backend = zsocket_new (ctx, ZMQ_DEALER);
	zsocket_bind (backend, "inproc://backend");
	int thread_nbr;
	for (thread_nbr = 0; thread_nbr < MAX_THREAD_NUM; thread_nbr++)
		zthread_fork (ctx, disposeData, psKeyWordCtrl);

	zmq_proxy (frontend, backend, NULL);
	zctx_destroy (&ctx);
}
