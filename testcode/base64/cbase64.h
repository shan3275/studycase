#ifndef __cbase64_h__
#define __cbase64_h__

#include <stdio.h>

#if __cplusplus
extern "C"{
#endif
    
    int base64_encode(const char *indata, int inlen, char *outdata, int *outlen);
    int base64_decode(const char *indata, int inlen, char *outdata, int *outlen);
            
#if __cplusplus
}
#endif

#endif /* base64_h */
