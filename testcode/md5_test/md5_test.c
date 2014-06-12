/*
** sample £ºmd5lib.h Ê¹ÓÃÀý×Ó*/
#include <stdio.h>
#include <stdlib.h>

#include "md5.h"
int main(int argc, char** argv)
{

char * text, *key ;

printf("string:%s\n",MDString(""));
printf("string:%s\n",MDString ("a"));
printf("string:%s\n",MDString ("abc"));
printf("string:%s\n",MDString ("message digest"));
printf("string:%s\n", MDString ("abcdefghijklmnopqrstuvwxyz"));
printf("string:%s\n",MDString("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"));
printf("string:%s\n", MDString("Has anyone altered the contents of this file?"));
if(argc > 1)
   printf("file %s MD5:%s\n", argv[1], MDFile(argv[1]));
//printf("string:%s\n", MDFile("ss2.upgrade.tar"));
 key ="Jefe";
 text ="what do ya want for nothing?";
 printf("ntext:%s\n",text);
 printf("key:%s\n",key);
 printf("string:%s\n",hmac_md5(text,key));
 return (0);
}
