#ifndef _TOOL_REDIS_UTIL_H_
#define _TOOL_REDIS_UTIL_H_
#include "hiredis.h"

typedef struct ToolRedisUtil
{
	redisContext * m_pRediusContext;	

	char m_sIp[16];
	int m_iPort;
	void (*initToolRedis)(struct ToolRedisUtil* psRedisUtil);
	void (*hsetToolRedisValue)(struct ToolRedisUtil* psRedisUtil, char* sCommand);
	bool (*existsToolRedis)(struct ToolRedisUtil* psRedisUtil, char* pcKey);
	void (*closeToolRedis)(struct ToolRedisUtil* psRedisUtil);
	int (*hgetToolRedisInt)(struct ToolRedisUtil* psRedisUtil, char* sCommand, int* result);
	void (*smemberToolRedisStr)(struct ToolRedisUtil* psRedisUtil, char* pcKey, char** pcStrs);
	int (*getToolRedisInt)(struct ToolRedisUtil* psRedisUtil, char* pcKey, int* result);
	void (*setToolRedisInt)(struct ToolRedisUtil* psRedisUtil, char* pcKey, int value);
	int (*hgetToolRedisStr)(struct ToolRedisUtil* psRedisUtil, char* sCommand, char* pcResult);
}SToolRedisUtil;

struct ToolRedisUtil* createToolRediusUtil(const char* sIp , int nPort);

void initToolRedis(struct ToolRedisUtil* psRedisUtil);

void hsetToolRedisValue(struct ToolRedisUtil* psRedisUtil, char* sCommand);

//void saddToolRedisStr(struct ToolRedisUtil* psRedisUtil, char* pcADUA,  zlist_t* adlist);

void smemberToolRedisStr(struct ToolRedisUtil* psRedisUtil, char* pcKey, char** pcStrs);

bool existsToolRedis(struct ToolRedisUtil* psRedisUtil, char* pcKey);

int hgetToolRedisInt(struct ToolRedisUtil* psRedisUtil, char* sCommand, int* result);

int getToolRedisInt(struct ToolRedisUtil* psRedisUtil, char* pcKey, int* result);

void setToolRedisInt(struct ToolRedisUtil* psRedisUtil, char* pcKey, int value);

int hgetToolRedisStr(struct ToolRedisUtil* psRedisUtil, char* sCommand, char* pcResult);

void closeToolRedis(struct ToolRedisUtil* psRedisUtil);

#endif
