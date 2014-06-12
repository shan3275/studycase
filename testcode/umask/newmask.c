/*
*  newmask.c - Chang the umask
*/
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>

int main(void)
{
    mode_t newmask = 0222, oldmask;

    system("touch before");
    oldmask = umask(newmask);
    printf("old umask is %#o\n", oldmask);
    printf("new umask is %#o\n", newmask);
    system("touch after");
    exit(EXIT_SUCCESS);
}

