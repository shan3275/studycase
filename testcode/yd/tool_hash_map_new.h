#ifndef __HASH_MAP_INCLUDE__
#define __HASH_MAP_INCLUDE__

struct TOOL_HASH_NODE
{
        unsigned int hashA;
        unsigned int hashB;
        long* value;
        struct TOOL_HASH_NODE * next;
};

typedef struct SHashMap
{
        unsigned int hashLen;
        unsigned int size;
        struct TOOL_HASH_NODE** ppHashNode;
}HashMap;


unsigned int hash_map_get_hash_number(char *start, int strlen, int hashLen);

unsigned long hash_map_get_hash(char *start, int len, unsigned long dwHashType);
/*
 * destroy hash map
 */

void hash_map_destory(HashMap* sHashMap);

void hash_map_item_delete(HashMap* sHashMap, char* key, int len);

long* hash_map_item_get(HashMap* sHashMap, int nHashPos);

int hash_map_find(HashMap* psHashMap, char*start, int len, long* out_val);

int hash_map_insert(HashMap* psHashMap, char*start, int len, long* in_val);

void hash_map_clear(HashMap* psHashMap);
int hash_map_size(HashMap* psHashMap);

/*
 * init hash table
 */
HashMap* hash_map_new(unsigned int length);

void hash_map_read_file(HashMap* psHashmap, const char * ccFilename);

HashMap* load_nolen_limit_hash_map(char* filename, int length);

HashMap* hash_map_load_file(const char * ccFilename, unsigned int length);

#endif
