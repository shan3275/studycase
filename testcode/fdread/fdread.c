/*
*  fdread.c - The read and write system calls
*/
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

int main(void)
{
    int fdsrc, fdnull, fdtmp, numbytes;
    int flags = O_CREAT | O_TRUNC | O_WRONLY;
    char buf[10];

    /* open the source file, /dev/null, and /tmp/foo.bar */
    if ((fdsrc = open("fdread.c", O_RDONLY, 0644)) < 0) {
            perror("open fdread.c");
            exit(EXIT_FAILURE);
    }

    if ((fdnull = open("/dev/null", O_WRONLY)) < 0) {
            perror("open /dev/null");
            close(fdsrc); /* close this since we've opened it */
            exit(EXIT_FAILURE);
    }

    if ((fdtmp = open("/dev/foo.bar", flags, 0644)) < 0) {
            perror("open /tmp/foo.bar");
            close(fdsrc); /* have to close both of these now */
            close(fdnull);
            exit(EXIT_FAILURE);
    }

    /* read and write 10 bytes at a time */
    while ((numbytes = read(fdsrc, buf, 10)) > 0) {
        if (write(fdnull, buf, 10) < 0) {
            perror("write /dev/null");
        }
        if (write(fdtmp, buf, numbytes) < 0) {
            perror("write /tmp/foo.bar");
        }
    }

    /* close files and exit */
    close(fdsrc);
    close(fdnull);
    close(fdtmp);

    exit(EXIT_SUCCESS);
}

