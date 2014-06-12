/*
*  pripds.c - Print PID and PPID
*/
#include <unistd.h>
#include <sys/times.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

void doit(char *, clock_t);

int main(void)
{
    clock_t start, end;
    struct tms t_start, t_end;
    long tps = sysconf(_SC_CLK_TCK);
    printf("ticks:%d\n",tps);
    start = times(&t_start);
    /* redirect output to prevent screen clutter */
    system("grep passwd -r /etc 2>&1 ./info.log");
    end = times(&t_end);

    doit("elapsed", end - start);

    puts("parent times");
    doit("\tuser CPU", t_end.tms_utime);
    doit("\tsys  CPU", t_end.tms_stime);

    puts("child times");
    doit("\tuser CPU", t_end.tms_cutime);
    doit("\tsys  CPU", t_end.tms_cstime);


    exit(EXIT_SUCCESS);
}

void doit (char *str, clock_t time)
{
    /* Get clock ticks/second */
    long tps = sysconf(_SC_CLK_TCK);

    printf("%s: %6.2f secs\n", str, (float)time/tps);
}

