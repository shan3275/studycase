#include "czmq.h"
#include "global.h"
#include "util.h"

int curr_time()
{
	time_t timep;
	time(&timep);
	return timep;
}

void server_worker (void *args, zctx_t *ctx, void *pipe)
{
	struct timeval start, end;
	void *worker = zsocket_new (ctx, ZMQ_DEALER);
	void *dsp_sender = zsocket_new (ctx, ZMQ_PUSH);
	zsocket_connect (worker, "inproc://backend");
	zsocket_connect (dsp_sender, "tcp://127.0.0.1:9002");
	
	int i = 0;
	//int curr = curr_time();
	while (true) {
		zmsg_t *msg = zmsg_recv (worker);
		if(!msg)
			break;
		zframe_t *identity = zmsg_pop (msg);
		zframe_t *content = zmsg_pop (msg);
		byte *data = zframe_data (identity);
		byte *con = zframe_data (content);
		zmsg_destroy (&msg);
		//发送给dsp平台
		zmq_send(dsp_sender, con, strlen(con), 1);

		MQ_Msg message = {{'\0'}, {'\0'}, {'\0'}, {'\0'}, {'\0'}};
		gettimeofday( &start, NULL );
		int ret = mq_msg_parser( con, &message);
		gettimeofday( &end, NULL );
		int timeuse = 1000000 * ( end.tv_sec - start.tv_sec ) + end.tv_usec -start.tv_usec;
		printf("time: %d us\n", timeuse);
		if(ret < 0)
		{
			continue;
		}
		printf("ip : %s, ua : %s, host : %s, key_word : %s, ts : %s\n", message.ip, message.UA, message.host, message.key_word, message.ts);
		char key[2064] = {0};
		char cKeyMD5s[33] = {'\0'};
		sprintf(key, "%s%s", message.ip, message.UA);
		rule_pop_encrypt_key_md5(key, cKeyMD5s);
	
		char command[1664] = {0};
		sprintf(command, "sadd %s  %sMEG%sMEG%s", cKeyMD5s, message.host, message.key_word, message.ts);
		printf("command : %s\n", command);
		//log4c_category_log(mycat, LOG4C_PRIORITY_DEBUG, "command : %s\n", command);
		/*if(redis_send_command(command)<0)
		{
			printf("redis command [%s] failed\n", command);
			//log4c_category_log(mycat, LOG4C_PRIORITY_ERROR, "redis command [%s] failed\n", command);
			redis_close();
			if(redis_init(g_redis_ip, g_redis_port)<0)
			{
				log4c_category_log(mycat, LOG4C_PRIORITY_ERROR, "redis command [%s] failed\n", command);
				//printf("redis server conn failed\n");
			}*/
		zframe_destroy(&identity);
		zframe_destroy(&content);
	}
}

