#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <getopt.h>

#include "ssctest.h"

char *program;

cmd_t command_list[] =
{
    {"test", 1, cmd_test, cmd_test_usage, "TEST"},
    {NULL, 0, NULL, NULL, NULL}
};

void usage(char *cmd)
{
    int f = 0;
    cmd_t *cmdp;

    int verbose = 0;

    if (cmd) {
        verbose = !strcmp("verbose", cmd);
    }

        printf("Usage: %s COMMAND [OPTIONS]\n", program);

    if (cmd == NULL || verbose) {
        printf("\nCOMMANDS:\n");
        for (cmdp = command_list; cmdp->name; cmdp++) {
            printf("    %-10s %s\n", cmdp->name, cmdp->disc);
        }
        f = 1;
    }


    for (cmdp = command_list; cmdp->name; cmdp++) {

        if (verbose || (cmd && !strncasecmp(cmdp->name, cmd, strlen(cmd) > cmdp->ncmp ? strlen(cmd) : cmdp->ncmp))) {
            printf("\n");
            (*cmdp->usage)();
            f = 1;
        }
    }


    if (f == 0 && cmd != NULL) {
        printf("Error: No command \"%s\"!\n", cmd);
    }

        exit(1);
}

int
cmd_help(int argc, char **argv)
{
    if (argc > 1) {
        usage(argv[1]);
    } else {
        usage("verbose");
    }

    return 0;
}

void
cmd_help_usage(void)
{
    printf("%s help [Commad]\n", program);
    printf("    -- display verbose help message.\n");
}

int main(int argc, char **argv)
{
    int rt = 0;
    cmd_t *cmdp;

    program = argv[0];

    if (argc < 2) {
        usage(NULL);
    }

    for (cmdp = command_list; cmdp->name; cmdp++) {
        if (!strncasecmp(cmdp->name, argv[1], strlen(argv[1]) > cmdp->ncmp ? strlen(argv[1]) : cmdp->ncmp)) {
            rt = (*cmdp->func)(argc - 1, &argv[1]);
        }
    }

    if (rt != 0) {
        printf("%s.%d: rt=%d\n", __func__, __LINE__, rt);
    }

    return rt;
}



/* End of file */


