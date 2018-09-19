
#ifndef __CPCRE_H__	
#define __CPCRE_H__	

void ngx_strupper(unsigned char *dst, unsigned char *src, size_t n);

int cpcre_idfa_init() ;

int cpcre_indo_idfa_match(char* idfa, char* matchstr) ;

void cpcre_idfa_close() ;

#endif

