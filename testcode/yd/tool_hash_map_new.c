#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "tool_hash_map_new.h"

unsigned long g_tool_crypt_table[0x1000];
unsigned int *g_all_position = NULL;
/*
 * init table of hash key
*/
static void hash_map_init_crypt_table()
{
    static int called = 0;
    if(called)
        return;
    else
        called = 1;

    unsigned long seed = 0x00100001, index1 = 0, index2 = 0, i;
    for( index1 = 0; index1 < 0x100; index1++ )
    {
        for( index2 = index1, i = 0; i < 5; i++, index2 += 0x100 )
        {
            unsigned long temp1, temp2;
            seed = (seed * 125 + 3) % 0x2AAAAB;
            temp1 = (seed & 0xFFFF) << 0x10;
            seed = (seed * 125 + 3) % 0x2AAAAB;
            temp2 = (seed & 0xFFFF);
            g_tool_crypt_table[index2] = ( temp1|temp2 );
        }
    }
}

/*
 * host hash
*/
unsigned long hash_map_get_hash(char *start, int len, unsigned long dwHashType)
{
    //printf("%s\n", *hostname);
    unsigned char *key  = (unsigned char *)(start);
    unsigned long seed1 = 0x7FED7FED;
    unsigned long seed2 = 0xEEEEEEEE;
    int ch;

    int i = 0;
    while(( i < len ) && (*key != '\0'))
    {
        i++;
        ch = *key++;
        seed1 = g_tool_crypt_table[(dwHashType << 8) + ch] ^ (seed1 + seed2);
        seed2 = ch + seed1 + seed2 + (seed2 << 5) + 3;
    }
    return seed1;
}

unsigned int hash_map_get_hash_number(char *start, int strlen, int hashLen)
{
        return hash_map_get_hash(start, strlen, 0) % hashLen;
}

unsigned int hash_map_hash_value(char* start, int len)
{
        unsigned int nHash  = hash_map_get_hash(start, len, 0);
        return nHash;
}

/*
 * destroy hash map
 */

void hash_map_destory(HashMap* sHashMap)
{
	int i = 0;
	struct TOOL_HASH_NODE ** tool_hash_table = sHashMap->ppHashNode;
	for(; i < sHashMap->hashLen; i++ )
	{
		if(tool_hash_table[i])
		{
			struct TOOL_HASH_NODE * currNode = tool_hash_table[i];
			struct TOOL_HASH_NODE * deleteNode = currNode;
			while(currNode)
			{
				if(currNode->value != NULL)
				{
					free(currNode->value);
					currNode->value = NULL;
				}
				currNode = currNode->next;
			}
			free(deleteNode);
			deleteNode = NULL;
		}
	}
	free(tool_hash_table);
	tool_hash_table = NULL;
	
	free(sHashMap);
	sHashMap = NULL;
}


void hash_map_item_delete(HashMap* sHashMap, char* key, int len)
{
	struct TOOL_HASH_NODE ** tool_hash_table = sHashMap->ppHashNode;
	unsigned int nHash  = hash_map_get_hash(key, len, 0);
	unsigned int nHashPos = nHash % sHashMap->hashLen;
	if (tool_hash_table[nHashPos])
	{
		struct TOOL_HASH_NODE * currNode = tool_hash_table[nHashPos];
		free(currNode->value);
		currNode->value = NULL;
		free(currNode);
		currNode = NULL;
		sHashMap->size--;
	}
} 


long* hash_map_item_get(HashMap* sHashMap, int nHashPos)
{
	struct TOOL_HASH_NODE ** tool_hash_table = sHashMap->ppHashNode;
	if (tool_hash_table[nHashPos])
	{
		struct TOOL_HASH_NODE * currNode = tool_hash_table[nHashPos];
		return currNode->value;
	}
	return NULL;
} 
/*
 * check by hash A B, if not exist then insert
 */
static int hash_map_check_append(HashMap* sHashMap, char *start, int len, long * in_val,
        int bInsert, long * out_val, unsigned int hashLen)
{
	struct TOOL_HASH_NODE ** tool_hash_table = sHashMap->ppHashNode;
    const int HASH_OFFSET = 0, HASH_A = 1, HASH_B = 2;
    unsigned int nHash  = hash_map_get_hash(start, len, HASH_OFFSET);
    unsigned int nHashA = hash_map_get_hash(start, len, HASH_A    );
    unsigned int nHashB = hash_map_get_hash(start, len, HASH_B    );
    unsigned int nHashStart = nHash % hashLen;
	unsigned int nHashPos = nHashStart;
	
    if (tool_hash_table[nHashPos])
    {
        struct TOOL_HASH_NODE * currNode = tool_hash_table[nHashPos];
        while(currNode)
        {
            if (currNode->hashA  == nHashA && currNode->hashB == nHashB)
            {
                if(bInsert)
                {
                    if(out_val != NULL)
                        currNode->value = out_val;
                }
                else
                {
                    //if(out_val != NULL)
                        *out_val = (long)currNode->value;
                }
                return nHashPos; //find the same host
            }
            currNode = currNode->next;
        }
    }

    if(bInsert)
    {
        //printf("hashtable: insert a %.*s\n", len, start);
        struct TOOL_HASH_NODE * nextNode = (struct TOOL_HASH_NODE*)malloc(sizeof(struct TOOL_HASH_NODE));
        nextNode->hashA = nHashA;
        nextNode->hashB = nHashB;
        nextNode->value = in_val;
		sHashMap->size++;
        if (tool_hash_table[nHashPos])
        {
            nextNode->next = tool_hash_table[nHashPos]; //add host to first
            tool_hash_table[nHashPos] = nextNode;
        }
        else
        {
            nextNode->next = NULL;
            tool_hash_table[nHashPos] = nextNode;  //add a new host
        }

        return 0;
    }

    return -1;
}


int hash_map_find(HashMap* psHashMap, char*start, int len, long* out_val)
{
	return hash_map_check_append(psHashMap, start, len, NULL, 0, out_val, psHashMap->hashLen);
}

int hash_map_insert(HashMap* psHashMap, char*start, int len, long* in_val)
{
	return hash_map_check_append(psHashMap, start, len, in_val, 1, in_val, psHashMap->hashLen);
}

void hash_map_clear(HashMap* psHashMap)
{
	memset(psHashMap->ppHashNode, 0, sizeof(struct TOOL_HASH_NODE *)*psHashMap->hashLen);
	free(psHashMap);
}
int hash_map_size(HashMap* psHashMap)
{
	return psHashMap->size;
}

/*
 * init hash table
 */
HashMap* hash_map_new(unsigned int length)
{
    hash_map_init_crypt_table();
	HashMap* psHashMap = (HashMap*) malloc(sizeof(HashMap));  
	if (!psHashMap)
	{
		printf("hash_map_new shashMap error.");
		exit(-1);
	}
	
    psHashMap->ppHashNode = (struct TOOL_HASH_NODE**)malloc(sizeof(struct TOOL_HASH_NODE *)*length);
	if (!psHashMap->ppHashNode)
	{
		free(psHashMap);
		printf("hash_map_new psHashMap->hashNode error.");
		exit(-1);
	}
	
    memset(psHashMap->ppHashNode, 0, sizeof(struct TOOL_HASH_NODE *)*length);
	
	psHashMap->hashLen = length;
	psHashMap->size = 0;
    return psHashMap;
}

/************************************************************************/
//  读取文件                                                                  
/************************************************************************/
void hash_map_read_file(HashMap* psHashmap, const char * ccFilename)
{
	FILE *fp= fopen(ccFilename,"r");
	if(!fp) return;
	while(!feof(fp))
	{
		char wd[255]={'\0'};
		fgets(wd, 255, fp);
		if(strlen(wd) < 5) continue;
		wd[strlen(wd) - 1] = '\0';
		hash_map_insert(psHashmap, wd, strlen(wd), 0);
		//printf("tool hash table load: %s\n", wd);

	}
	fclose(fp);
}

/************************************************************************/
//  读取网站白名单文件                                                                  
/************************************************************************/
void hash_map_host_white_read_file(HashMap* psHashmap, const char * ccFilename)
{
	FILE *fp= fopen(ccFilename,"r");
	if(!fp) return;
	while(!feof(fp))
	{
		char wd[255]={'\0'};
		fgets(wd, 255, fp);
		if(strlen(wd) < 5)
		{
			continue;
		}
		wd[strlen(wd) - 1] = '\0';

		if (strstr(wd, "suning") || strstr(wd, "taobao") || strstr(wd, "gome") || strstr(wd, "yhd") || strstr(wd, "gov")
			|| strstr(wd, "tuniu") || strstr(wd, "cntv") || strstr(wd, ".people.com") || strstr(wd, ".china.com")
			|| strstr(wd, "api") || strstr(wd, "plugin"))
		{
			printf("host white file have black site: %s\n", wd);
			continue;
		}

		hash_map_insert(psHashmap, wd, strlen(wd), 0);
		//printf("tool hash table load: %s\n", wd);
	}
	fclose(fp);
}

void hash_map_read_dict_file(HashMap* psHashmap, const char * ccFilename)
{
	FILE *fp= fopen(ccFilename,"r");
	if(!fp) return;
	while(!feof(fp))
	{
		char wd[255]={'\0'};
		fgets(wd, 255, fp);
		if(strlen(wd) < 5) continue;
		wd[strlen(wd) - 1] = '\0';
		char key[192] = {0};
                char value[64] = {0};
                memcpy(key, wd, strstr(wd,"=")-wd);
                memcpy(value, strstr(wd,"=")+1, strlen(wd)-strlen(key)-1);
		hash_map_insert(psHashmap, wd, strlen(wd), 0);
		//printf("tool hash table load: %s\n", wd);

	}
	fclose(fp);
}
HashMap* load_nolen_limit_hash_map(char* filename, int length)
{
        HashMap* psHashmap = hash_map_new(length);
        FILE *fp= fopen(filename,"rb");
        if(!fp) return NULL;
        while(!feof(fp))
        {
                char wd[255]={'\0'};
                fgets(wd, 255, fp);
                wd[strlen(wd) - 1] = '\0';
                hash_map_insert(psHashmap, wd, strlen(wd), 0);
        }
        fclose(fp);
        return psHashmap;
}

/************************************************************************/
//  [7/30/2014 fushengli]                                               
//  将文件的内容载入到hashmap                                                                  
/************************************************************************/
HashMap* hash_map_load_file(const char * ccFilename, unsigned int length)
{
	HashMap* psHashmap = hash_map_new(length);
	hash_map_read_file(psHashmap, ccFilename);
	return psHashmap;
}
