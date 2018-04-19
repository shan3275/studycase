/**************************************************************
* Copyright (C) 2014-2018 All rights reserved.
* @Version: 1.0
* @Created: 2018-04-04 18:01
* @Author: SamLiu - shan3275@gmail.com
* @Description: 
*
* @History: 
**************************************************************/
#include "cbase64.h"
int main()
{
    int rv=0;
    char *indata="https://baike.baidu.com/item/sprintf/9703430?fr=aladdin";
    char outdata[2048];
    int outlen =0;
    memset(outdata,0,2048);
    rv =  base64_encode(indata, strlen(indata), outdata, &outlen);
    if (rv)
    {
        printf("rv=%d\n",rv);
    }
    printf("indata: %s\n", indata);
    printf("outdata: %s\n", outdata);
}
