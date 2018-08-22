#ifndef _HOVERLEES_INI_CONFIG_H
#define _HOVERLEES_INI_CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
//#include <unistd.h>

typedef struct _CONFIG_BTREE_NODE{
	char*	key;
	void*	data;
	struct _CONFIG_BTREE_NODE* left;
	struct _CONFIG_BTREE_NODE* right;
	char mem[2];
}CONFIG_BTREE_NODE;

typedef struct _CONFIG_BTREE{
	int numNodes;
	CONFIG_BTREE_NODE* root;
}CONFIG_BTREE;

typedef CONFIG_BTREE INI_CONFIG;

typedef int (*CONFIG_BTREE_TRAVERSE_CB)(CONFIG_BTREE_NODE* node);
typedef int (*CONFIG_BTREE_SAVE_TRAVERSE_CB)(FILE* fp,CONFIG_BTREE_NODE* node);

INI_CONFIG* ini_config_create_from_string(unsigned char* str,int slen,int isGBK);

INI_CONFIG* ini_config_create_from_file(const char* filename,int isGBK);

void ini_config_destroy(INI_CONFIG* config);
int ini_config_get_int(INI_CONFIG* config,const char* section,const char* key,int default_int);
char* ini_config_get_string(INI_CONFIG* config,const char* section,const char* key,char* default_string);
int ini_config_set_string(INI_CONFIG* config,const char* section,const char* key,int key_len,const char* value,int value_len);
int ini_config_set_int(INI_CONFIG* config,const char* section,const char* key,int key_len,int value);
int ini_config_save(INI_CONFIG* config,const char* filename);
int ini_config_print(INI_CONFIG* config,FILE* fp);

#endif
