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
    int qid; /* The queue identifier */
    int len; /* Length of data sent */
    struct msg pmsg; /* Pointer to message structure */

    /* file operation */
    int fdsrc;
    int flags = O_RDONLY;
    char buf[10];

    /* file operation */
    /* open the source file, /dev/null, and /tmp/foo.bar */
    memset(buf, 0, 10);
    if ((fdsrc = open("../config", flags, 0644)) < 0) {
            perror("open ../config");
            exit(EXIT_FAILURE);
    }

    /* read qid from file */
    if (read(fdsrc, buf, 10) < 0) {
            perror("read ../config failed");
    }

    /* Expect the queue ID passed on the command line */
    qid = atoi(buf);
    printf("qid:%d\n", qid);

    /* Get the message to add to the queue */
    puts("Enter message to post:");
    if ((fgets((&pmsg)->msg_text, BUFSZ, stdin)) == NULL) {
        puts("no message to post");
        exit(EXIT_FAILURE);
    }

    /* Associate the message with this process */
    pmsg.msg_type = getpid();
    printf("message type: %05ld\n", (&pmsg)->msg_type);
    /* Add the message to the queue */
    len = strlen(pmsg.msg_text);
    if ((msgsnd(qid, &pmsg, len, 0)) < 0) {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }
    puts("message posted");

    exit(EXIT_SUCCESS);
}
