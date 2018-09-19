
#include <pcre.h>
#include <string.h>


#define ngx_tolower(c)      (u_char) ((c >= 'A' && c <= 'Z') ? (c | 0x20) : c)
#define ngx_toupper(c)      (u_char) ((c >= 'a' && c <= 'z') ? (c & ~0x20) : c)
#define OVECCOUNT 128    
static pcre *re_indo_idfa;


//下面这段代码来自nginx  core/ngx_string
void ngx_strupper(unsigned char *dst, unsigned char *src, size_t n)
{
    while (n) {
        *dst = ngx_toupper(*src);
        dst++;
        src++;
        n--;
    }
}


int cpcre_idfa_init()
{
        const char *error;
        int erroffset;

	char pattern_indo_idfa[] = "((_i|i|m.i|pub_i)dfa|o_iosadvid|ios_ifa|u|gaid|s5|ifa|adid|device(_id|id|ifa|\[ifa\])|kv19|I|D|d\(id24\)|(unique|vc)_user_id|imei|qyid|androidid|uid|did.(i|ia)|ud|p_device.dpid|iosadid|imprId)=[a-zA-Z0-9]{8}-([0-9a-zA-Z]{4}-){3}[a-zA-Z0-9]{12}";

        re_indo_idfa = pcre_compile(pattern_indo_idfa, 0, &error, &erroffset, NULL);

        if (re_indo_idfa==NULL) {
                printf("PCRE compilation indo idfa pattern failed at offset %d: %s\n", erroffset, error);
                return -1;
        }

	return 0;
}


int cpcre_indo_idfa_match(char* idfa, char* matchstr)
{
        int ovector[OVECCOUNT];
        int rcCM = pcre_exec(re_indo_idfa, NULL, idfa, strlen(idfa), 0, 0, ovector, OVECCOUNT);
        if(rcCM<0)
        {
                if (rcCM==PCRE_ERROR_NOMATCH)
                {
                        //printf("no match\n");
			return -1;
                }
                //printf("Matching error %d\n", rcCM);
		return -1;
        }
		char* p = NULL;
		int ret = pcre_get_substring(idfa, ovector, rcCM, 0, &p);  
        	memcpy( matchstr, p, ret );
        	pcre_free_substring(p);
        if (rcCM > 0) {

                //printf("String : %s\n", idfa);
		return 0;
        }

	return -1;
}

void cpcre_idfa_close()
{
	free(re_indo_idfa);	
}


