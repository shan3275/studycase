#include "init.h"
#include "tool_redis_util.h"

struct ToolRedisUtil* createToolRediusUtil( const char* sIp, int nPort)
{
	struct ToolRedisUtil* util = (struct ToolRedisUtil*)malloc(sizeof(struct ToolRedisUtil));
	if(NULL  == util)
	{
		//printf("malloc ToolRedisUtil failed\n");
		dzlog_error("error", "malloc ToolRedisUtil failed\n");
		return NULL;
	}	
	memset(util, 0, sizeof(struct ToolRedisUtil));
	memcpy(util->m_sIp, sIp, strlen(sIp));
	util->m_iPort = nPort;
	util->initToolRedis = initToolRedis;
	util->hsetToolRedisValue = hsetToolRedisValue;
	util->hgetToolRedisInt = hgetToolRedisInt;
	util->hgetToolRedisStr = hgetToolRedisStr;
	util->getToolRedisInt = getToolRedisInt;
	util->setToolRedisInt = setToolRedisInt;
	util->smemberToolRedisStr = smemberToolRedisStr;
	util->existsToolRedis = existsToolRedis;
	util->closeToolRedis = closeToolRedis;
	return util; 
}

static void check_context(struct ToolRedisUtil* psRedisUtil)
{
	int     nfds;   /* socket and # file descriptors */
	struct timeval tv={3,0};
	fd_set  arfds ;
	int fd = psRedisUtil->m_pRediusContext->fd;
	nfds = getdtablesize();
	FD_ZERO(&arfds);
	FD_SET(fd, &arfds);
	if(-1 == select(fd + 1, &arfds, &arfds, NULL, &tv))
	{
		redisFree(psRedisUtil->m_pRediusContext);
		psRedisUtil->m_pRediusContext = NULL;
		psRedisUtil->initToolRedis( psRedisUtil );	
	}
}

void initToolRedis(struct ToolRedisUtil* psRedisUtil)
{
	psRedisUtil->m_pRediusContext = redisConnect(psRedisUtil->m_sIp, psRedisUtil->m_iPort);
	if(psRedisUtil->m_pRediusContext->err)
	{
		//printf("connection error: %d", psRedisUtil->m_pRediusContext->err);
		dzlog_error("error", "connection error: %d", psRedisUtil->m_pRediusContext->err);
		redisFree(psRedisUtil->m_pRediusContext);
		psRedisUtil->m_pRediusContext = NULL;
	}
}

void hsetToolRedisValue(struct ToolRedisUtil* psRedisUtil, char* sCommand)
{
	check_context(psRedisUtil);
	redisReply *reply = redisCommand(psRedisUtil->m_pRediusContext, sCommand);
	if (reply->type == REDIS_REPLY_ERROR) 
	{  
		//printf("hsetToolRedis failed to execute hset %s, error : %s\n", sCommand, reply->str);  
		dzlog_error("error", "hsetToolRedis failed to execute hset %s, error : %s\n", sCommand, reply->str);  
	}
	else
	{
		//printf("hsetToolRedis success to execute hset %s type=%d\n", sCommand, reply->type);
		dzlog_info("debug","hsetToolRedis success to execute hset %s type=%d\n", sCommand, reply->type);
	}
	freeReplyObject(reply);  
}


bool existsToolRedis(struct ToolRedisUtil* psRedisUtil, char* pcKey)
{
	check_context(psRedisUtil);
	redisReply *reply = redisCommand(psRedisUtil->m_pRediusContext, "exists %s", pcKey);
	if (reply->type == REDIS_REPLY_ERROR) 
	{  
                freeReplyObject(reply);
                //printf("existsToolRedis failed  to execute [%s]\n", pcKey);
                dzlog_error("error","existsToolRedis failed  to execute [%s]\n", pcKey);
                return false;

	}
	else if(reply->type == REDIS_REPLY_INTEGER) 
	{
		freeReplyObject(reply);
		return true;
	}
	
}

void smemberToolRedisStr(struct ToolRedisUtil* psRedisUtil, char* pcKey, char** pcStrs)
{
	check_context(psRedisUtil);
	redisReply *reply = redisCommand(psRedisUtil->m_pRediusContext, "smembers %s", pcKey);
	if (NULL != reply && reply->type == REDIS_REPLY_ARRAY) 
	{  
		int i =0 ;
		for(; i < reply->elements; i++)
		{
			memcpy(pcStrs[i], reply->element[i]->str, strlen(reply->element[i]->str));
		}
		//printf("smember %s get element len=%d.\n", pcKey, i);  
		dzlog_info("debug","smember %s get element len=%d.\n", pcKey, i);  
	}
	freeReplyObject(reply); 
}
/*
void saddToolRedisStr(struct ToolRedisUtil* psRedisUtil, char* pcADUA, zlist_t* adlist)
{
	check_context(psRedisUtil);
	int iCommandNum = 0;
	char pcCommand[2048] = {'\0'};
	//printf("saddToolRedisStr execute sadd command key is %s%s.\n", pcAD, pcUA);

	char *Advid = NULL;
	for(Advid = zlist_head(adlist); Advid != NULL; Advid = zlist_next(adlist))
        {
		sprintf(pcCommand, "sadd %s %s", pcADUA, Advid);
		printf("redis : %s\n", pcCommand);
		redisReply *reply = redisCommand(psRedisUtil->m_pRediusContext, pcCommand);
		if (NULL != reply && reply->type != REDIS_REPLY_INTEGER) 
		{  
			printf("Failed to execute sadd command[%s].\n", pcCommand);  
		}
		freeReplyObject(reply); 
		iCommandNum++;
        }

}
*/
int getToolRedisInt(struct ToolRedisUtil* psRedisUtil, char* pcKey, int* result)
{
	check_context(psRedisUtil);
	redisReply *reply;
	reply = redisCommand(psRedisUtil->m_pRediusContext, "get %s", pcKey);
	if (NULL != reply && reply->type == REDIS_REPLY_STRING)
	{
		*result = atoi(reply->str);
		//printf("getToolRedisInt success get %s=%s\n", pcKey, reply->str);
		dzlog_info("debug", "getToolRedisInt success get %s=%s\n", pcKey, reply->str);
	}
	else
	{
		//printf("getToolRedisInt failed get %s.\n", pcKey); 
		dzlog_error("error","getToolRedisInt failed get %s.\n", pcKey); 
		freeReplyObject(reply); 
		return -1;
	}

	freeReplyObject(reply);
	return 1;
}

void setToolRedisInt(struct ToolRedisUtil* psRedisUtil, char* pcKey, int value)
{
	check_context(psRedisUtil);
	redisReply *reply;
	reply = redisCommand(psRedisUtil->m_pRediusContext, "set %s %d", pcKey, value);
	if (NULL != reply && reply->type == REDIS_REPLY_STATUS && strcasecmp(reply->str,"OK") == 0) 
	{  
		//printf("setToolRedisInt success to execute set %s %d.\n", pcKey, value);  
		dzlog_info("debug","setToolRedisInt success to execute set %s %d.\n", pcKey, value);  
	}
	else
	{
		//printf("setToolRedisInt failed to execute set %s %d.\n", pcKey, value);
		dzlog_error("error","setToolRedisInt failed to execute set %s %d.\n", pcKey, value);
	}

	freeReplyObject(reply);
}

int hgetToolRedisInt(struct ToolRedisUtil* psRedisUtil, char* sCommand, int* result)
{
	check_context(psRedisUtil);
	redisReply *reply;
	reply = redisCommand(psRedisUtil->m_pRediusContext, sCommand);
	if (NULL != reply && reply->type == REDIS_REPLY_STRING)
	{
		*result = atoi(reply->str);
		//printf("hgetToolRedis success %s=%s\n", sCommand, reply->str);
		dzlog_info("debug", "hgetToolRedis success %s=%s\n", sCommand, reply->str);
	}
	else
	{
		//printf("hgetToolRedis failed command[%s].\n", sCommand); 
		dzlog_error("error","hgetToolRedis failed command[%s].\n", sCommand); 
		freeReplyObject(reply); 
		return -1;
	}

	freeReplyObject(reply);
	return 1;
}

int hgetToolRedisStr(struct ToolRedisUtil* psRedisUtil, char* sCommand, char* pcResult)
{
	check_context(psRedisUtil);
	redisReply *reply;
	reply = redisCommand(psRedisUtil->m_pRediusContext, sCommand);
	if (NULL != reply && reply->type == REDIS_REPLY_STRING)
	{
		memcpy(pcResult, reply->str, strlen(reply->str));
		//printf("hgetToolRedis success %s=%s\n", sCommand, reply->str);
	}
	else
	{
		//printf("hgetToolRedis failed command[%s].\n", sCommand); 
		freeReplyObject(reply); 
		return -1;
	}

	freeReplyObject(reply);
	return 1;
}

void closeToolRedis(struct ToolRedisUtil* psRedisUtil)
{
	redisFree(psRedisUtil->m_pRediusContext);
	free(psRedisUtil);
	psRedisUtil = NULL;
}

//static int SendCommand( char* sCommand, char* szOut)
//{
//	redisReply *reply = redisCommand(tool_reids_context, sCommand);
//	if (NULL == reply) {
//		printf("Failed to execute command[%s]. reason : redisCOmmand return NULL\n", sCommand);
//		freeReplyObject(reply);
//		return -1;
//	}
//	if((REDIS_ERR == reply) && (strcasecmp(tool_reids_context->err,REDIS_ERR_EOF) == 0))
//	{
//		printf("Failed to execute command[%s]. reason : reply conn have expire\n",sCommand);
//		freeReplyObject(reply);
//		return -1;
//	}
//	if(NULL != reply && reply->type == REDIS_REPLY_ARRAY)
//	{
//		int i =0 ;
//		char szBuff[64] = {0};
//		for(; i< reply->elements; i++)
//		{
//			if((i % 2) ==0)
//			{
//				memset(szBuff,'\0', 64);
//				strcpy(szBuff, reply->element[i]->str);
//			}
//			else
//			{
//				strcat(szBuff, reply->element[i]->str);
//				memcpy(szOut, szBuff, strlen(szBuff));
//			}
//		}
//	}
//	freeReplyObject(reply);
//	return 0;
//}
//
//void radius_redis_load_init()
//{
//	ConnRedis("127.0.0.1" , 6379);
//}

//int radius_redis_getall(char* szOut)
//{
//	if(SendCommand("select 3", NULL)<0)
//	{
//		printf("select failed\n");
//		return -1;
//	}
//	if(SendCommand("hgetall cxxIPUA", szOut)<0)
//	{
//		printf("hgetall failed\n");
//		return -1;
//	}
//	return 0;
//}
