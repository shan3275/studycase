/*
*  pending.c - Fun with sigpending
*/
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

void err_quit(char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(void)
{
    sigset_t set, pendset;
    struct sigaction action;

    /* Create the set */
    if ((sigemptyset(&set)) < 0) {
        err_quit("sigemptyset");
    }
    /* Add the interesting signal */
    if ((sigaddset(&set, SIGTERM)) < 0) {
        err_quit("sigaddset:SIGTERM");
    }

    /* Block the signal */
    if ((sigprocmask(SIG_BLOCK, &set, NULL)) < 0) {
        err_quit("sigprocmask");
    }

    /* Send SIGTERM to myself */
    kill(getpid(), SIGTERM);

    /* Get pending signals */
    sigpending(&pendset);

    /* If SIGTERM pending, ignore it */
    if (sigismember(&pendset, SIGTERM)) {
        sigemptyset(&action.sa_mask);
        action.sa_handler = SIG_IGN; /* Ignore SIGTERM */
        sigaction(SIGTERM, &action, NULL);
    }

    /* Unblock SIGTERM*/
    sigprocmask(SIG_UNBLOCK, &set, NULL);
    printf("over!\n");
    exit(EXIT_SUCCESS);
}

