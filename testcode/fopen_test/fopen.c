/*
*  fdopen.c - opening and closing file descriptors
*/
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

int main(void)
{
    int fd;
    char path[] = "hello";

    for (;;)
    {
       if ((fd = open(path, O_CREAT | O_TRUNC | O_WRONLY, 0644)) < 0) {
           perror("open");
           exit(EXIT_FAILURE);
        }else{
              printf("opened %s\n",path);
              printf("descriptor is %d\n",fd);
        }

    }

    exit(EXIT_SUCCESS);
}

