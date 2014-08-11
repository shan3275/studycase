#include "sts_cli.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "libcli.h"
#include "md_sts.h"

static int sts_cmd_get (struct cli_def *cli, char *command, char *argv[], int argc)
{
    md_pkt_sts sts;
    int rv;

    memset((void *)&sts, 0, sizeof(md_pkt_sts));
    rv = md_sts_get( &sts );
    if (rv)
    {
        cli_print(cli, "ERROR rv=%d", rv);
        return CLI_ERROR;
    }
    
    cli_print(cli, "input   %d", sts.get);
    cli_print(cli, "fwd     %d", sts.fwd);
    cli_print(cli, "drop    %d", sts.drop);
    cli_print(cli, "permit  %d", sts.permit);
    cli_print(cli, "free    %d", sts.free);

    return CLI_OK;
}

static int sts_cmd_clear (struct cli_def *cli, char *command, char *argv[], int argc)
{
    int rv;

    rv = md_sts_clear();
    if (rv)
    {
        cli_print(cli, "ERROR rv=%d", rv);
        return CLI_ERROR;
    }

    return CLI_OK;
}

int sts_cmd_init( struct cli_def *cli)
{
    int flag_ifgrp_work = 0;

    struct cli_command *c;

    /* statistics  */
    c = cli_register_command(cli, NULL, "sts", NULL, PRIVILEGE_UNPRIVILEGED,
        MODE_EXEC, "statistics");
    cli_register_command(cli, c, "get", sts_cmd_get, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "get");

    cli_register_command(cli, c, "clear", sts_cmd_clear, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "clear");
    return CLI_OK;
}

