#include "md_if.h"
#include "md_ifgrp.h"
#include "if_cli.h"

static int interface_cmd_link (struct cli_def *cli, char *command, char *argv[], int argc, int flag)
{
    int rv;
    uint8_t if_id;

    if_id   = strtoul ( argv[0], NULL, 0 );
    cli_print(cli, "if_id:%d", if_id);
    rv = if_set_if_phy ( if_id, flag);
    if ( rv )
    {
        cli_print(cli, "rv=%d\n", rv);
        return CLI_ERROR;
    }
    return CLI_OK;
}

int interface_cmd_init( struct cli_def *cli)
{
    int link  = 0;

    struct cli_command  *c1, *c;
    char tmpstr_if[128];

    sprintf ( tmpstr_if,    "<0-%d>", MAX_IF_NUM - 1 );

    /* add input interface group member */
    c  = cli_register_command(cli, NULL, "interface", NULL, PRIVILEGE_UNPRIVILEGED,
        MODE_EXEC, "input interface group");
    c1 = cli_register_command(cli, c, tmpstr_if, NULL, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "interface id");

    link = 1;
    cli_register_command_f(cli, c1, "link", interface_cmd_link, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "interface up", link);

    link = 0;
    cli_register_command_f(cli, c1, "down", interface_cmd_link, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "interface up", link);

    return CLI_OK;
}

int interface_show_one(struct cli_def *cli, uint8_t if_id)
{
    int rv;
    uint8_t online,link,ifgrp,weight;

    rv = if_get_if_online(if_id, &online);
    if (rv)
    {
        cli_print(cli, "ERROR rv=%d", rv);
        return CLI_ERROR;
    }

    rv = if_get_if_link(if_id, &link);
    if (rv)
    {
        cli_print(cli, "ERROR rv=%d", rv);
        return CLI_ERROR;
    }

    rv = if_get_if_iifgrp(if_id, &ifgrp);
    if (rv)
    {
        cli_print(cli, "ERROR rv=%d", rv);
        return CLI_ERROR;
    }

    rv = if_get_if_weight(if_id, &weight);
    if (rv)
    {
        cli_print(cli, "ERROR rv=%d", rv);
        return CLI_ERROR;
    }

    cli_print(cli, "interface id:%d, %s, %s, iifgpr:%d, weight:%d",
            if_id, online?"online":"offline", link?"link":"down", ifgrp, weight);

    return CLI_OK;
}

static int interface_cmd_show (struct cli_def *cli, char *command, char *argv[], int argc)
{
    uint8_t if_id;

    if_id =  strtoul ( argv[0], NULL, 0 );
    cli_print(cli, "grp_id:%d\n", if_id);

    interface_show_one(cli, if_id);
    return CLI_OK;
}

static int interface_cmd_show_all (struct cli_def *cli, char *command, char *argv[], int argc)
{
    int i;

    for ( i = 0; i < MAX_IF_NUM; i++ )
    {
        interface_show_one(cli, i);
    }

    return CLI_OK;
}

int interface_cmd_show_init( struct cli_def *cli, struct cli_command *root)
{
    struct cli_command *c1;
    char tmpstr_if[128];

    sprintf ( tmpstr_if, "<0-%d>", MAX_IF_NUM - 1 );

    /* show input interface group member */
    c1 = cli_register_command(cli, root, "interface", NULL, PRIVILEGE_UNPRIVILEGED,
        MODE_EXEC, "input interface ");
    cli_register_command(cli, c1, tmpstr_if, interface_cmd_show, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "group id");
    cli_register_command(cli, c1, "all", interface_cmd_show_all, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "all interface");

    return CLI_OK;
}
