#ifndef __FS_MD5_H__
#define __FS_MD5_H__

typedef unsigned char *POINTER; /* POINTER defines a generic pointer type */
typedef unsigned short int UINT2; /* UINT2 defines a two byte word */
typedef unsigned long int UINT4; /* UINT4 defines a four byte word */

typedef struct {
    UINT4 state[4];    /* state (ABCD) */
    UINT4 count[2];        /* number of bits, modulo 2^64 (lsb first) */
    unsigned char buffer[64]; /* input buffer */
} MD5_CTX;

#define MD5_DIGEST_SZ   16
#define MD5_DIGEST_STR_SZ   ((MD5_DIGEST_SZ * 2) + 1)

void MD5Init(MD5_CTX *context);
char* MDFile(char *filename);
void MD5Update(MD5_CTX *context, unsigned char *input,unsigned int inputLen);
void MD5Final(unsigned char digest[MD5_DIGEST_SZ], MD5_CTX *context);
void MD5FD(int fd, unsigned char digest[MD5_DIGEST_SZ]);
void MD5_digest_dump(unsigned char digest[MD5_DIGEST_SZ]);
void MD5_digest_str(unsigned char digest[MD5_DIGEST_SZ], char str[MD5_DIGEST_STR_SZ]);
void MD5FD_LEN(int fd, unsigned char digest[MD5_DIGEST_SZ], unsigned int remain);

#endif /* !__FS_MD5_H__ */
