#ifndef __UTIL_H__
#define __UTIL_H__


#include "init.h"

#define MAX_IP_LEN 32
/*
 * ori : 原始字符串
 * des : 目标字符串
 * ori_separator : 原始字符串的分隔符
 * des_separator : 目标字符串的分隔符
 * */
void str_split(const char* ori, char* des, char* ori_separator, char* des_separator); //ori can not change  

/*
 * char *string;
 * char *fields[];                       list is not NULL-terminated 
 * int nfields;                   number of entries available in fields[] 
 * char *sep;                     "" white, "c" single char, "ab" [ab]+ 
 * return value                           number of fields, including overflow 
 * */
int split(char *string, char* fields[], int nfields,char* sep);

long* find_pid_by_name( char* pidName);

void inet_ntoa_r(unsigned int ip, char ipstr[MAX_IP_LEN]);

int get_file_time(char *sFileName, char* sFileTime);

int timevaltochartime(time_t tv_sec, char * out_time);

void strtohex(u_char *dest, u_char *src, int nLen);
#endif
