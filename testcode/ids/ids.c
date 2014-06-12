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
    printf("Real user ID = %d\n",getuid());
    printf("Effective user ID = %d\n",geteuid());
    printf("Real group ID = %d\n",getgid());
    printf("Effcetive group ID = %d\n",getegid());
    exit(EXIT_SUCCESS);
}

