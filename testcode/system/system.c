/*
*  system.c - Demonstrate the system() call
*/

#include <stdlib.h>
#include <stdio.h>


int main(void)
{
    int retval;
        retval = system("ls -l");
        if (retval == 127) {
            fprintf(stderr,"/bin/sh not availabel\n");
            exit(127);
        } else if (retval == -1) {
            perror("system");
            exit(EXIT_FAILURE);
        } else if (retval != 0) {
            fprintf(stderr,"command returned %d\n",retval);
            perror("ls");
        } else {
            puts("command successfully executed");
        }
    exit(EXIT_SUCCESS);
}

