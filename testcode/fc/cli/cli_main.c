#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include "cli_cmd.h"
#include "md_shm.h"

extern struct cli_def *cli ;
char *l_opt_arg;

static const char *shortopts = "hd";
struct option longopts[] = {
{"help", no_argument, NULL, 'h'},
{"daemon", no_argument, NULL, 'd'},
{0, 0, 0, 0},
};

/* Display program usage, and exit.
 *  */
void display_usage( char *process )
{
    printf("Usage:\n" );
    printf(" %s [options]\n", process);
    printf("Options:\n");
    printf(" -d, --daemon          put process to background\n");
    printf(" -h, --help            display this help text and exit\n");
}

int main (int argc, char *argv[])
{
    int c;
    int debug = 1;
    int rv;

    while ((c = getopt_long (argc, argv, shortopts, longopts, NULL)) != -1)
    {
      switch (c)
       {
       case 'h':
          display_usage(argv[0]);
          break;
       case 'd':
          debug = 0;
          break;
       }
    }

    if (!debug)
    {
        daemon(0, 0);
    }

    rv = md_get_shm();
    if (rv)
    {
        printf("Get SHM FAIL!\n");
    }

    rv = loop_if_monitor();
    if (rv)
    {
        printf("crreate if monitor fail!\n");
    }

    loop_server();

    return 0;
}
