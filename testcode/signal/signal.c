#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include <errno.h>
#if 0
int sec= 0;
void sigroutine(int signo){

        int tt = sec++;
    switch (signo){
    case SIGALRM:
        printf(" 1 Catch a signal -- SIGALRM tt = %d\n" ,tt);
        //signal(SIGALRM, sigroutine);
        fflush(stdout);
        sleep(10);
        printf(" 2 Catch a signal -- SIGALRM tt = %d\n" ,tt);
        fflush(stdout);
        break;
    case SIGVTALRM:
        printf("Catch a signal -- SIGVTALRM \n");
        signal(SIGVTALRM, sigroutine);
        break;
    }
    return;
}

int main()
{
    struct itimerval value, ovalue;
    //struct itimerval value2;

    printf("process id is %d\n", getpid());
    signal(SIGALRM, sigroutine);
    //signal(SIGVTALRM, sigroutine);
    value.it_value.tv_sec = 1;
    value.it_value.tv_usec = 0;
    value.it_interval.tv_sec = 1;
    value.it_interval.tv_usec = 0;
    setitimer(ITIMER_REAL, &value, &ovalue);

    //value2.it_value.tv_sec = 0;
    //value2.it_value.tv_usec = 500000;
    //value2.it_interval.tv_sec = 0;
    //value2.it_interval.tv_usec = 500000;
    //setitimer(ITIMER_VIRTUAL, &value2, &ovalue);
    for(;;)
        ;
}

#else
int nolock = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
long int ticks;
void sigroutine(int signo){

    int ret;
    switch (signo){
    case SIGALRM:
        //signal(SIGALRM, sigroutine);
        fflush(stdout);
        /* Try to lock the mutex */
        ret = pthread_mutex_trylock(&mutex);
        if (ret != EBUSY) {
            if (ret != 0) {
                perror("pthread_mutex_trylock");
                exit(EXIT_FAILURE);
            }
            printf("mon_th: got lock at %ld ticks\n", ticks);
            if (pthread_mutex_unlock(&mutex) != 0) {
                perror("pthread_mutex_unlock");
                exit(EXIT_FAILURE);
            }
        } else {
            /* Number of times mutex was locked */
            nolock++;
            printf("mon_th missed lock %d times\n", nolock);
        }
        fflush(stdout);
        break;
    case SIGVTALRM:
        printf("Catch a signal -- SIGVTALRM \n");
        signal(SIGVTALRM, sigroutine);
        break;
    }
    return;
}

int main()
{
    struct itimerval value, ovalue;

    printf("process id is %d\n", getpid());
    signal(SIGALRM, sigroutine);
    value.it_value.tv_sec = 1;
    value.it_value.tv_usec = 0;
    value.it_interval.tv_sec = 1;
    value.it_interval.tv_usec = 0;
    setitimer(ITIMER_REAL, &value, &ovalue);

    for(;;) {
        /* Lock the mutex */
        if (pthread_mutex_lock(&mutex) != 0) {
            perror("pthread_mutex_lock");
            exit(EXIT_FAILURE);
        }
        ++ticks;
        /* Now we're done, so unlock the mutex */
        if (pthread_mutex_unlock(&mutex) != 0) {
            perror("pthread_mutex_unlock");
            exit(EXIT_FAILURE);
        }

        sleep(1);
        //usleep(1);
    }
}
#endif
