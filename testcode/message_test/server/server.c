#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#define SSCTEST_DEBUG 0

#if SSCTEST_DEBUG
#define SSCTEST_DBG(fmt, args...) printf("[DEBUG]%s.%d: " fmt, __FUNCTION__, __LINE__, ##args)
#else
#define SSCTEST_DBG(fmt, args...)
#endif
#define FTOK_GET_KEY
#ifdef FTOK_GET_KEY
#define FTOK_PATH "/etc/passwd"
#define FTOK_SERVER_ID 0
#define FTOK_CLIENT_ID 1
#endif
void server_init(void);
typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;
/* Message structure */
typedef struct{
    long msg_type;
    uint8_t op;
    uint32_t reg;
    uint32_t value;
}msg;

uint32_t data=0x1234;
int qid_server,qid_client; /* The queue identifier */
#define OP_NULL     0
#define OP_READ     1
#define OP_WRITE    2
#define OP_VERSION  3
#define OP_SETTING  4
#define OP_STAT     5
#ifndef FTOK_GET_KEY
void server_init(void)
{
    key_t key_server,key_client; /* The queue key */
    /* file operation */
    int fdsrc, numbytes;
    int flags = O_CREAT | O_TRUNC | O_WRONLY;
    char buf[10];

    key_server = 123;
    key_client = 456;

    /* Create the server queue */
    if ((qid_server = msgget(key_server, IPC_CREAT | 0666)) < 0) {
        perror("msgget:create");
        exit(EXIT_FAILURE);
    }
    printf("create server queue id = %d\n", qid_server);

    /* Open the server queue again */
    if ((qid_server = msgget(key_server, 0)) < 0) {
        perror("msgget:open");
        exit(EXIT_FAILURE);
    }
    printf("opened server queue id = %d\n", qid_server);

    /* file operation */
    /* open the source file */
    memset(buf, 0, 10);
    numbytes = sprintf(buf, "%d", qid_server);
    if ((fdsrc = open("../config.server", flags, 0644)) < 0) {
            perror("open ../config.server");
            exit(EXIT_FAILURE);
    }

    /* write qid to file */
    if (write(fdsrc, buf, numbytes) < 0) {
            perror("write ../config.server failed");
    }

    /* close files and exit */
    close(fdsrc);

    /* Create the client queue */
    if ((qid_client = msgget(key_client, IPC_CREAT | 0666)) < 0) {
        perror("msgget:create");
        exit(EXIT_FAILURE);
    }
    printf("create client queue id = %d\n", qid_client);

    /* Open the client queue again */
    if ((qid_client = msgget(key_client, 0)) < 0) {
        perror("msgget:open");
        exit(EXIT_FAILURE);
    }
    printf("opened client queue id = %d\n", qid_client);

    /* file operation */
    /* open the source file */
    memset(buf, 0, 10);
    numbytes = sprintf(buf, "%d", qid_client);
    if ((fdsrc = open("../config.client", flags, 0644)) < 0) {
            perror("open ../config.client");
            exit(EXIT_FAILURE);
    }

    /* write qid to file */
    if (write(fdsrc, buf, numbytes) < 0) {
            perror("write ../config.client failed");
    }

    /* close files and exit */
    close(fdsrc);

}

#else
void server_init(void)
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

    /* Create the server queue */
    if ((qid_server = msgget(key_server, IPC_CREAT | 0666)) < 0) {
        perror("msgget:create");
        exit(EXIT_FAILURE);
    }
    printf("create server queue id = %d\n", qid_server);

    /* Open the server queue again */
    if ((qid_server = msgget(key_server, 0)) < 0) {
        perror("msgget:open");
        exit(EXIT_FAILURE);
    }
    printf("opened server queue id = %d\n", qid_server);

    /* Create the client queue */
    if ((qid_client = msgget(key_client, IPC_CREAT | 0666)) < 0) {
        perror("msgget:create");
        exit(EXIT_FAILURE);
    }
    printf("create client queue id = %d\n", qid_client);

    /* Open the client queue again */
    if ((qid_client = msgget(key_client, 0)) < 0) {
        perror("msgget:open");
        exit(EXIT_FAILURE);
    }
    printf("opened client queue id = %d\n", qid_client);

}
#endif

int main(int argc, char *argv[])
{
    pid_t child;
    int len; /* Length of message */
    msg pmsg; /* A message structure */
    server_init();
    if ((child = fork()) == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (child == 0) {
        /* Retrieve and display a message from the queue */
        for (;;) {
            len = msgrcv(qid_server, &pmsg, sizeof(msg)-sizeof(long), 0, 0);
            if (len > 0) {
                SSCTEST_DBG("reading queue id: %05d\n", qid_server);
                SSCTEST_DBG("message type: %05ld\n", (&pmsg)->msg_type);
                SSCTEST_DBG("message length: %d bytes\n", len);
                SSCTEST_DBG("message op: %s\n", (&pmsg)->op==OP_WRITE?"WR":"RD");
                SSCTEST_DBG("message reg:0x%.4x\n",(&pmsg)->reg);
                switch (pmsg.op) {
                case OP_READ:
                    (&pmsg)->value = data;
                    printf("RD message value:0x%.4x\n",(&pmsg)->value);
                    if ((msgsnd(qid_client, &pmsg, len, 0)) < 0) {
                        perror("msgsnd");
                        exit(EXIT_FAILURE);
                    }
                    break;
                case OP_WRITE:
                    data = pmsg.value;
                    printf("WR message value:0x%.4x\n",(&pmsg)->value);
                    if ((msgsnd(qid_client, &pmsg, len, 0)) < 0) {
                        perror("msgsnd");
                        exit(EXIT_FAILURE);
                    }
                    break;
                default:
                    break;
                }


            } else {
                perror("msgrcv");
                exit(EXIT_FAILURE);
            }
            sleep(1);
        }

            exit(EXIT_SUCCESS);
        } else {
            puts("in parent");
            printf("\tparent pid = %d\n", getpid());
            printf("\tparent ppid = %d\n", getppid());
        }
    exit(EXIT_SUCCESS);
}
