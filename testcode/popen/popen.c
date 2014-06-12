/*
 * popen.c - Using popen() to open a pipe
 */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <limits.h>

#define BUFSZ PIPE_BUF

void err_quit(char *msg);

int main(void)
{
    FILE *fp; /* FILE stream for popen */
    char *cmdstring = "cat popen.c";
    char buf[BUFSZ];

    /* Create the pipe */
    if ((fp = popen(cmdstring, "r")) == NULL) {
        err_quit("pipe");
    }

    /* Read cmdstring's output */
    while ((fgets(buf, BUFSZ, fp)) != NULL) {
        printf("%s", buf);
    }

    /* Close and frap the exit status */
    pclose(fp);
    exit(EXIT_SUCCESS);
}

void err_quit(char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}
