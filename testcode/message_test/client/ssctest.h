#ifndef _SSCTEST_H
#define _SSCTEST_H

#include <sys/time.h>
#include <time.h>

/*----------------------------------- CONSTANTS ----------------------------------------*/
#define OP_NULL     0
#define OP_READ     1
#define OP_WRITE    2
#define OP_VERSION  3
#define OP_SETTING  4
#define OP_STAT     5


/*---------------------------------- ENUMERATIONS --------------------------------------*/


/*--------------------------- STRUCTURE/UNION DATA TYPES -------------------------------*/
typedef struct {
    char *name;
    int ncmp;
    int (*func)(int argc, char **argv);
    void (*usage)(void);
    char *disc;
} cmd_t;

typedef unsigned char uchar;
typedef unsigned long long u64;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;
/*--------------------------------- GLOBAL DATA ----------------------------------------*/
extern char *program;
extern cmd_t command_list[];

/*----------------------------- FUNCTION PROTOTYPE(S) ----------------------------------*/
void usage(char *cmd);

int cmd_help(int argc, char **argv);
void cmd_help_usage(void);

int cmd_test(int argc, char **argv);
void cmd_test_usage(void);

/*------------------------------------ MACROS ------------------------------------------*/

#define HARGTS(_val, _min, _max) \
{ \
    char *e; \
    unsigned int _vali;\
    if (optarg != NULL) { \
        _vali = strtoul(optarg, &e, 16); \
        if (e == optarg || *e != '\0') { \
            printf("Error: invail format!\n"); \
            usage(argv[0]); \
        } \
        if (_vali < _min ) { \
            printf("Error: out of range!\n"); \
            usage(argv[0]); \
        } \
        if (_max > 0 && _vali > _max) { \
            printf("Error: out of range!\n"); \
            usage(argv[0]); \
        } \
        _val = _vali & 0xffff; \
    } \
}

#define HARGTC(_val, _min, _max) \
{ \
    char *e; \
    unsigned int _vali;\
    if (optarg != NULL) { \
        _vali = strtoul(optarg, &e, 16); \
        if (e == optarg || *e != '\0') { \
            printf("Error: invail format!\n"); \
            usage(argv[0]); \
        } \
        if (_vali < _min ) { \
            printf("Error: out of range!\n"); \
            usage(argv[0]); \
        } \
        if (_max > 0 && _vali > _max) { \
            printf("Error: out of range!\n"); \
            usage(argv[0]); \
        } \
        _val = _vali & 0xff; \
    } \
}

#define HARGTI(_val, _min, _max) \
{ \
    char *e; \
    if (optarg != NULL) { \
        _val = strtoul(optarg, &e, 16); \
        if (e == optarg || *e != '\0') { \
            printf("Error: invail format!\n"); \
            usage(argv[0]); \
        } \
        if (_val < _min ) { \
            printf("Error: out of range!\n"); \
            usage(argv[0]); \
        } \
        if (_max > 0 && _val > _max) { \
            printf("Error: out of range!\n"); \
            usage(argv[0]); \
        } \
    } \
}

#define HARGTL(_val) \
{ \
    char *e; \
    if (optarg != NULL) { \
        _val = strtoull(optarg, &e, 16); \
        if (e == optarg || *e != '\0') { \
            printf("Error: invail format!\n"); \
            usage(argv[0]); \
        } \
    } \
}


#define DARGTI(_val, _min, _max) \
{ \
    char *e; \
    if (optarg != NULL) { \
        _val = strtoul(optarg, &e, 10); \
        if (e == optarg || *e != '\0') { \
            printf("Error: invail format!\n"); \
            usage(argv[0]); \
        } \
        if (_val < _min ) { \
            printf("Error: out of range!\n"); \
            usage(argv[0]); \
        } \
        if (_max > 0 && _val > _max) { \
            printf("Error: out of range!\n"); \
            usage(argv[0]); \
        } \
    } \
}
#define SSCTEST_DEBUG 1

#if SSCTEST_DEBUG
#define SSCTEST_DBG(fmt, args...) printf("[DEBUG]%s.%d: " fmt, __FUNCTION__, __LINE__, ##args)
#else
#define SSCTEST_DBG(fmt, args...)
#endif
#endif /* End of _SSCTEST_H */



