#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <string.h>
#include "ssctest.h"
#define FTOK_GET_KEY
#ifdef FTOK_GET_KEY
#define FTOK_PATH "/etc/passwd"
#define FTOK_SERVER_ID 0
#define FTOK_CLIENT_ID 1
#endif

/* Message structure */
typedef struct{
    long msg_type;
    uint8_t op;
    uint32_t reg;
    uint32_t value;
}msg;

int qid_server,qid_client; /* The queue identifier */
void client_init(void);
void ssctest_msg_read(uint32_t reg, uint32_t *value)
{
    msg pmsg; /* Pointer to message structure */
    int len;

    memset(&pmsg, 0, sizeof(pmsg));
    /* Associate the message with this process */
    pmsg.msg_type = getpid();
    pmsg.op       = OP_READ;
    pmsg.reg      = reg;

    SSCTEST_DBG("message type: %05ld\n", (&pmsg)->msg_type);
    SSCTEST_DBG("message op: %s\n", (&pmsg)->op==OP_WRITE?"WR":"RD");
    SSCTEST_DBG("message reg:0x%.4x\n",(&pmsg)->reg);
    /* Add the message to the queue */
    len = sizeof(msg)-sizeof(long);
    if ((msgsnd(qid_server, &pmsg, len, 0)) < 0) {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }
    SSCTEST_DBG("message posted");
    for (;;) {
        len = msgrcv(qid_client, &pmsg, sizeof(msg)-sizeof(long), getpid(), 0);
        SSCTEST_DBG("message recved");
        if (len > 0) {
            if (pmsg.msg_type==getpid()) {
                SSCTEST_DBG("message type: %05ld\n", (&pmsg)->msg_type);
                SSCTEST_DBG("message length: %d bytes\n", len);
                SSCTEST_DBG("message op: %s\n", (&pmsg)->op==OP_WRITE?"WR":"RD");
                SSCTEST_DBG("message reg:0x%.4x\n",(&pmsg)->reg);
                SSCTEST_DBG("message value:0x%.4x\n",(&pmsg)->value);
                *value = pmsg.value;
                break;
            }
        } else {
                perror("msgrcv");
                exit(EXIT_FAILURE);
        }
    }
}

void ssctest_msg_write(uint32_t reg, uint32_t value)
{
    msg pmsg; /* Pointer to message structure */
    int len;

    memset(&pmsg, 0, sizeof(pmsg));
    /* Associate the message with this process */
    pmsg.msg_type = getpid();
    pmsg.op       = OP_WRITE;
    pmsg.reg      = reg;
    pmsg.value    = value;

    SSCTEST_DBG("message type: %05ld\n", (&pmsg)->msg_type);
    SSCTEST_DBG("message op: %s\n", (&pmsg)->op==OP_WRITE?"WR":"RD");
    SSCTEST_DBG("message reg:0x%.4x\n",(&pmsg)->reg);
    SSCTEST_DBG("message value:0x%.4x\n",(&pmsg)->value);
    /* Add the message to the queue */
    len = sizeof(msg)-sizeof(long);
    if ((msgsnd(qid_server, &pmsg, len, 0)) < 0) {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }
    SSCTEST_DBG("message posted\n");
    for (;;) {
        len = msgrcv(qid_client, &pmsg, sizeof(msg)-sizeof(long), 0, 0);
        SSCTEST_DBG("message recved\n");
        if (len > 0) {
            if (pmsg.msg_type==getpid()) {
                SSCTEST_DBG("message type: %05ld\n", (&pmsg)->msg_type);
                SSCTEST_DBG("message length: %d bytes\n", len);
                SSCTEST_DBG("message op: %s\n", (&pmsg)->op==OP_WRITE?"WR":"RD");
                SSCTEST_DBG("message reg:0x%.4x\n",(&pmsg)->reg);
                SSCTEST_DBG("message value:0x%.4x\n",(&pmsg)->value);
                break;
            }
        } else {
                perror("msgrcv");
                exit(EXIT_FAILURE);
        }
    }
}
#ifndef FTOK_GET_KEY
void client_init(void)
{
    /* file operation */
    int fdsrc;
    int flags = O_RDONLY;
    char buf[10];

    /* file operation */
    /* open the source file, /dev/null, and /tmp/foo.bar */
    memset(buf, 0, 10);
    if ((fdsrc = open("../config.server", flags, 0644)) < 0) {
            perror("open ../config.server");
            exit(EXIT_FAILURE);
    }

    /* read qid from file */
    if (read(fdsrc, buf, 10) < 0) {
            perror("read ../config.server failed");
    }
    close(fdsrc);

    /* Expect the queue ID passed on the command line */
    qid_server = atoi(buf);
    SSCTEST_DBG("qid:%d\n", qid_server);


    /* file operation */
    /* open the source file, /dev/null, and /tmp/foo.bar */
    memset(buf, 0, 10);
    if ((fdsrc = open("../config.client", flags, 0644)) < 0) {
            perror("open ../config.client");
            exit(EXIT_FAILURE);
    }

    /* read qid from file */
    if (read(fdsrc, buf, 10) < 0) {
            perror("read ../config.server failed");
    }
    close(fdsrc);

    /* Expect the queue ID passed on the command line */
    qid_client = atoi(buf);
    SSCTEST_DBG("qid:%d\n", qid_client);
}

#else
void client_init(void)
{
    key_t key_server,key_client; /* The queue key */

    key_server = ftok(FTOK_PATH,FTOK_SERVER_ID);
    if (key_server < 0) {
        perror("crest key_server");
        exit(EXIT_FAILURE);
    }
    key_client = ftok(FTOK_PATH,FTOK_CLIENT_ID);
    if (key_client < 0) {
        perror("crest key_client");
        exit(EXIT_FAILURE);
    }
    /* Open the server queue again */
    if ((qid_server = msgget(key_server, 0)) < 0) {
        perror("msgget:open");
        exit(EXIT_FAILURE);
    }
    printf("opened server queue id = %d\n", qid_server);


    /* Open the client queue again */
    if ((qid_client = msgget(key_client, 0)) < 0) {
        perror("msgget:open");
        exit(EXIT_FAILURE);
    }
    printf("opened client queue id = %d\n", qid_client);

}
#endif

void
cmd_test_usage(void)
{
    printf("%s test (-R|-W) -a <Addr> [-v <Value>] \n", program);
    printf("    -- Read/write reg.\n");
    printf("    -R         Read.\n");
    printf("    -W         Write.\n");
    printf("    -a <Addr>  Addr of register; Hex format, Wide: 1 Byte, no default.\n");
    printf("    -v <Value> Value to write; Hex format, Wide: 1 Byte, default: 0.\n");
}

int
cmd_test(int argc, char **argv)
{
    int opt;
    int action = OP_NULL;
    uint32_t addr = 0, value = 0;
    int len; /* Length of data sent */
    if (argc < 2) {
        usage(argv[0]);
    }
    client_init();
    while ((opt = getopt(argc, argv, "RWa:v:h?")) != -1) {
        switch (opt) {
        case 'R':
            action = OP_READ;
            break;
        case 'W':
            action = OP_WRITE;
            break;
        case 'a':
            HARGTS(addr, 0x0, 0xffff);
            break;
        case 'v':
            HARGTS(value, 0x0, 0xffff);
            break;
        case 'h':
        case '?':
        default:
            usage(argv[0]);
            break;
        }
    }

    switch (action) {
    case OP_READ:
        ssctest_msg_read(addr, &value);
        printf("RD addr 0x%.4x, value 0x%.4x.\n", addr, value);
        break;
    case OP_WRITE:
        ssctest_msg_write(addr, value);
        printf("WR addr 0x%.4x, value 0x%.4x.\n", addr, value);
        break;
    default:
        usage(argv[0]);
        break;
    }

    return 0;
}
/* End of file */





