/*
 * piperw.c - Open and close a pipe
 */
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <limits.h>

#define BUFSZ PIPE_BUF

void err_quit(char *msg);

int main(int argc, char *argv[])
{
    int fd[2]; /* Array for file descriptors */
    int fdin; /* Descriptor for input file */
    char buf[BUFSZ];
    int pid, len;

    /* Create the pipe */
    if ((pipe(fd)) < 0) {
        err_quit("pipe");
    }
    printf("descirptors are %d, %d\n", fd[0], fd[1]);
    /* Fork and close the appropriate descriptors */
    if ((pid = fork()) < 0) {
        err_quit("fork");
    }
    if (pid == 0) {
        /* Child is reader, close the write descriptor */
        close(fd[1]);
        while ((len = read(fd[0], buf, BUFSZ)) > 0) {
            write(STDOUT_FILENO, buf, len);
        }
        close(fd[0]);
    } else {
        /* Parent is writer, close the read descriptor */
        close(fd[0]);
        if ((fdin = open(argv[1], O_RDONLY)) < 0) {
            perror("open");
            /* Send something since we couldn't open the input */
            write(fd[1], "123\n", 4);
        } else {
            while ((len = read(fdin, buf, BUFSZ)) > 0) {
                write(fd[1], buf, len);
            }
            close(fdin);
        }
        /* Close the write descriptor*/
        close(fd[1]);
    }
    /* Reap the exit status */
    waitpid(pid, NULL, 0);

    exit(EXIT_SUCCESS);
}

void err_quit(char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}
