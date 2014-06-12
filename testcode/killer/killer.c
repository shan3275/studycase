/*
*  killer.c - Killing other precesses
*/

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

int main(void)
{
    pid_t child;
    int status, retval;

    if ((child = fork()) < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (child == 0) {
        /* Sleep long enough to killed */
        sleep(1000);
        exit(EXIT_SUCCESS);
    } else {
        /* Use WNOHANG so wait will return */
        if ((waitpid(child, &status, WNOHANG)) == 0) {
            retval = kill(child, SIGKILL);
            if (retval) {
                /* Kill failed, so wait on child to exit */
                puts("kill failed\n");
                perror("kill");
                waitpid(child, &status, 0);
            } else
                printf("%d killed\n", child);
        }
    }
    exit(EXIT_SUCCESS);
}

