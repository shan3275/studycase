/*****************************************************************************************
gmtime()函数是将日历时间转化为世界标准时间（即格林尼治时间），
并返回一个tm结构体来保存这个时间，而localtime()函数是将日历时
间转化为本地时间。比如用localtime()函数在中国地区获得的本地时间
会比世界标准时间晚8个小时:
local time:2009/01/26   3:44:17
UTC time:2009/01/25   19:44:17

linux乐园 (www.linuxview.net)整理
License:GPL
E-mail:linuxdev@163.com
***************************************************************************************/

#include     <stdio.h>
#include     <stdlib.h>
#include     <string.h>
#include     <time.h>
#include     <unistd.h>
#include     <sys/time.h>
#include     <sys/times.h>

int main()
{
    time_t the_time;
    struct tm *tp;
    time(&the_time);
    tp=localtime(&the_time);

    printf("Local time:%d/%.2d/%.2d   %d:%.2d:%.2d\n",
        tp->tm_year+1900,tp->tm_mon+1,tp->tm_mday,
        tp->tm_hour,tp->tm_min,tp->tm_sec);

    tp=gmtime(&the_time);
    printf("UTC time:%d/%.2d/%.2d   %d:%.2d:%.2d\n",
        tp->tm_year+1900,tp->tm_mon+1,tp->tm_mday,
        tp->tm_hour,tp->tm_min,tp->tm_sec);

    return 0;
}
