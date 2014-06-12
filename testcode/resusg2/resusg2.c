/*
*  resusg2.c.c - Getting resource usage with getrusage
*/
#include <unistd.h>
#include <sys/times.h>
#include <time.h>
#include <sys/resource.h>
#include <stdlib.h>
#include <stdio.h>

void err_quit(char *);
void doit(char *, long);

int main(void)
{
    struct rusage usage;

    /* redirect output to prevnet screen clutter */
    system("grep passwd -r /etc 2>&1 ./info.log");

    /* get the resource structure for the parent */
    if ((getrusage(RUSAGE_SELF, &usage)) == -1) {
        err_quit("getrusage");
    }

    puts("parent times");
    doit("\tuser CPU", usage.ru_utime.tv_sec);
    doit("\tsys CPU", usage.ru_stime.tv_sec);

    puts("parent memory stats");
    doit("\tminor fault", usage.ru_minflt);
    doit("\tmajor fault", usage.ru_majflt);
    doit("\tpage  fault", usage.ru_nswap);

     /* get the resource structure for the child */
    if ((getrusage(RUSAGE_CHILDREN, &usage)) == -1) {
        err_quit("getrusage");
    }

    puts("child times");
    doit("\tuser CPU", usage.ru_utime.tv_sec);
    doit("\tsys CPU", usage.ru_stime.tv_sec);

    puts("child memory stats");
    doit("\tminor fault", usage.ru_minflt);
    doit("\tmajor fault", usage.ru_majflt);
    doit("\tpage  fault", usage.ru_nswap);

    exit(EXIT_SUCCESS);
}

void doit (char *str, long resval)
{
    printf("%s: %ld\n", str, resval);
}

void err_quit(char *str)
{
    perror(str);
    exit(EXIT_FAILURE);
}
