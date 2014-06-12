/*
 * opipe.c - Open and close a pipe
 */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(void)
{
    int fd[2]; /* Array for file descriptors */

    if ((pipe(fd)) < 0) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    printf("descirptors are %d, %d\n", fd[0], fd[1]);
    close(fd[0]);
    close(fd[1]);
    exit(EXIT_SUCCESS);
}

