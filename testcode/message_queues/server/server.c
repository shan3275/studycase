#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>


#define BUFSZ 512

/* Message structure */
struct msg {
    long msg_type;
    char msg_text[BUFSZ];
};

int main(int argc, char *argv[])
{
    pid_t child;
    int qid; /* The queue identifier */
    key_t key; /* The queue key */
    int len; /* Length of message */
    struct msg pmsg; /* A message structure */

    /* file operation */
    int fdsrc, numbytes;
    int flags = O_CREAT | O_TRUNC | O_WRONLY;
    char buf[10];

    key = 123;

    /* Create the queue */
    if ((qid = msgget(key, IPC_CREAT | 0666)) < 0) {
        perror("msgget:create");
        exit(EXIT_FAILURE);
    }
    printf("create queue id = %d\n", qid);

    /* Open the queue again */
    if ((qid = msgget(key, 0)) < 0) {
        perror("msgget:open");
        exit(EXIT_FAILURE);
    }
    printf("opened queue id = %d\n", qid);

    /* file operation */
    /* open the source file, /dev/null, and /tmp/foo.bar */
    memset(buf, 0, 10);
    numbytes = sprintf(buf, "%d", qid);
    if ((fdsrc = open("../config", flags, 0644)) < 0) {
            perror("open ../config");
            exit(EXIT_FAILURE);
    }

    /* write qid to file */
    if (write(fdsrc, buf, numbytes) < 0) {
            perror("write ../config failed");
    }

    /* close files and exit */
    close(fdsrc);

    if ((child = fork()) == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (child == 0) {
        /* Retrieve and display a message from the queue */
        for (;;) {
            len = msgrcv(qid, &pmsg, BUFSZ, 0, 0);
            if (len > 0) {
                (&pmsg)->msg_text[len] = '\0';
                printf("reading queue id: %05d\n", qid);
                printf("message type: %05ld\n", (&pmsg)->msg_type);
                printf("message length: %d bytes\n", len);
                printf("message text: %s\n", (&pmsg)->msg_text);
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
