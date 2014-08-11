#include "embrave_cli.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "libcli.h"
#include "md_embrave.h"

static int embrave_cmd_start (struct cli_def *cli, char *command, char *argv[], int argc)
{
    int rv;
    uint8_t num =  strtoul ( argv[0], NULL, 0 );
    rv = md_embrave_start ( num );
    if (rv)
    {
        return CLI_ERROR;
    }
    return CLI_OK;
}

int embrave_cmd_init( struct cli_def *cli)
{
    int flag_ifgrp_work = 0;

    struct cli_command *c, *c1, *c2, *c3;
    char tmpstr[128];

    sprintf ( tmpstr, "<1-%d>", MAX_EMBRAVE_NUM - 1 );

    /* add input interface group member */
    c = cli_register_command(cli, NULL, "embrave", NULL, PRIVILEGE_UNPRIVILEGED,
        MODE_EXEC, "input embrave");
    c1 = cli_register_command(cli, c, tmpstr, embrave_cmd_start, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "packet number");
    return CLI_OK;
}

