/*
*  pripds.c - Print PID and PPID
*/
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

int main(void)
{
    printf("PID = %d\n",getpid());
    printf("PPID = %d\n",getppid());
    printf("FID = %d\n",getuid());
    printf("EID = %d\n",geteuid());
    printf("FGID = %d\n",getgid());
    printf("EGID = %d\n",getegid());
    exit(EXIT_SUCCESS);
}

