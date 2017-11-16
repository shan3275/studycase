#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include<sys/time.h>
#include <signal.h>

/**
 *  alarm
 *  ¿¿¿¿¿¿¿¿¿¿¿¿ alarm() ¿ signal() ¿¿¿ 
 *  unsigned int alarm(unsigned int seconds) 
 *  ¿ ¿¿SIGALRM¿¿¿¿¿¿¿¿¿¿¿seconds¿¿¿¿¿¿¿¿¿¿¿SIGALRM¿¿¿¿¿¿¿¿¿¿¿
 *  ¿¿¿¿alarm¿¿¿¿¿¿¿ alarm()¿¿¿¿¿¿¿
 *  ¿¿¿¿seconds¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿
 *  ¿¿¿¿alarm¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿ ¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿0¿
 **/

void sigalrm_fn(int sig)
{
    /* Do something */
    printf("alarm!/n");
    alarm(2);
    return;
}

int main(void) 
{
    signal(SIGALRM, sigalrm_fn);
    alarm(2);

    /* Do someting */
    while(1) pause();
}

