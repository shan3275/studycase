/*
 * wrfifo.c - Write to a "Well-know" FIFO
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <limits.h>
#include <time.h>

int main(int argc, char *argv[])
{
    int fd;   /* Descriptor for FIFO */
    int len;  /* Bytes read from FIFO */
    char buf[PIPE_BUF];
    time_t tp;

    if (argc != 2) {
        puts("USAGE: fifoname {name}");
        exit(EXIT_FAILURE);
    }

    /* Identify myself */
    printf("I am %d\n", getpid());

    /* Open the FIFO write-only */
    if ((fd = open(argv[1], O_WRONLY)) < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    /* Generate some data to write */
    while (1) {
        /* Get the current time */
        time(&tp);

        /* Create the string to write */
        len = sprintf(buf, "wrfifo %d sends %s", getpid(), ctime(&tp));

        /*
         * Use (len +1) because sprintf does not count
         * the terminating null
         */
        if ((write(fd, buf, len+1)) < 0) {
            perror("write");
            close(fd);
            exit(EXIT_FAILURE);
        }
        sleep(3);
    }
    close(fd);

    exit(EXIT_SUCCESS);
}
