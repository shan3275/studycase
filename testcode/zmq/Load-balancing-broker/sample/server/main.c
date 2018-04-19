#include "global.h"
#include "cfgfileloader.h"
#include "keywordfilter.h"
#include "tool_redis_util.h"
#include "keywordctrl.h"

int main()
{
	if ( mylog_init() == 1 )
    	{
        	printf("mylog_init() failed!\n");
    	}	

	SCfgFileLoader* psFileLoader = newCfgFileLoader();
	psFileLoader->readCfgFile(psFileLoader);

	SKeyWordFilter* psKeyWordFilter = newKeyWordFilter(psFileLoader);
	if(NULL == psKeyWordFilter)
	{
		printf("create keyword filter failed\n");
		return -1;
	}
	psKeyWordFilter->initKeyWordFilter(psKeyWordFilter);

	SToolRedisUtil* psRedisUtil = createToolRediusUtil(psFileLoader->getRedisIp(psFileLoader), psFileLoader->getRedisPort(psFileLoader));
	psRedisUtil->initToolRedis(psRedisUtil);
	
	SKeyWordCtrl* psKeyWordCtrl = newKeyWordCtrl(psKeyWordFilter, psRedisUtil);

	psKeyWordCtrl->initZmqRouter(psKeyWordCtrl);

	if(mylog_fini() == 1)
    	{
        	printf("mylog_fini() failed!\n");
    	}	
	return 0;
}



