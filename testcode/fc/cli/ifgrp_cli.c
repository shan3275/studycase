#include "ifgrp_cli.h"
#include "md_ifgrp.h"

#define MAX_IF_GROUP_NUM  40
#define MAX_IF_NUM        40
#define MAX_WORK_PRIORITY 4

static int iifgrp_cmd_add (struct cli_def *cli, char *command, char *argv[], int argc)
{
    int rv;
    uint8_t grp_id, if_id;

    grp_id =  strtoul ( argv[0], NULL, 0 );
    if_id  =  strtoul ( argv[1], NULL, 0 );
    cli_print(cli, "grp_id:%d, if_id:%d", grp_id, if_id);
    rv = md_iifgrp_add(grp_id, if_id);
    if (rv)
    {
        cli_print(cli, "Error rv=%d", rv);
        return CLI_ERROR;
    }

    return CLI_OK;
}

static int iifgrp_cmd_del (struct cli_def *cli, char *command, char *argv[], int argc)
{
    int rv;
    uint8_t grp_id, if_id;

    grp_id =  strtoul ( argv[0], NULL, 0 );
    if_id  =  strtoul ( argv[1], NULL, 0 );
    cli_print(cli, "grp_id:%d, if_id:%d\n", grp_id, if_id);
    rv = md_iifgrp_del(grp_id, if_id);
    if (rv)
    {
        cli_print(cli, "Error rv=%d", rv);
        return CLI_ERROR;
    }
    return CLI_OK;
}

static int iifgrp_cmd_del_all (struct cli_def *cli, char *command, char *argv[], int argc)
{
    int rv;
    uint8_t grp_id;
    int i;

    grp_id =  strtoul ( argv[0], NULL, 0 );
    cli_print(cli, "grp_id:%d\n", grp_id);
    for ( i = 0; i < MAX_IF_NUM; i++ )
    {
        rv = md_iifgrp_del(grp_id, i);
        if (rv)
        {
            cli_print(cli, "Error rv=%d", rv);
        }
    }
    return CLI_OK;
}

char *ifgrp_work[MD_IF_GRP_WORK_MAX] = 
{
    "ACL    ",
    "IP     ",
    "STRING ",
    "PAYLOAD",
};

void iifgrp_show_one(struct cli_def *cli, uint8_t grp_id)
{
    int i;
    int index,bit;
    iif_grp grp;
    if_grp_member *grp_member = NULL;
    if_grp_work   *grp_work   = NULL;
    int rv;
    uint8_t in;

    md_iifgrp_get(grp_id, &grp);
    cli_print(cli, "input interface group %d", grp.if_grp_id);
    grp_member = &grp.grp_member;
    cli_print(cli, "%d members:", grp_member->if_grp_port_num);
    for ( i = 0; i < MAX_IF_NUM; i++ )
    {
        index = i / UINT64_BIT_NUM;
        bit   = i % UINT64_BIT_NUM;
        if ( grp_member->ifmask[index] & (1 << (UINT64_BIT_NUM - bit -1)) )
        {
            cli_print(cli, "%d ", i);
        }
    }

    grp_work = &grp.grp_work;
    for ( i = 0; i < MAX_GRP_ACL; i++ )
    {
        if ( grp_work->priority[i] != INVALID_IFGRP_WORK_INDEX)
        {
            cli_print(cli, "priority %d, work %s", i, ifgrp_work[grp_work->priority[i]]);
        }
    }

    for ( i = 0; i < MAX_GRP_ACL; i++ )
    {
        if ( grp_work->grp_acl[grp_work->master][i] != NULL )
        {
            rv = md_if_grp_search_work_table(grp_work->grp_acl[grp_work->master][i], &in);
            if ( rv )
            {
                cli_print(cli, "ERROR rv=%d", rv);
            }
            if (in != INVALID_IFGRP_WORK_INDEX)
            {
                cli_print(cli, "%d, work %s", i, ifgrp_work[in]);
            }
        }
    }
}

static int iifgrp_cmd_show (struct cli_def *cli, char *command, char *argv[], int argc)
{
    uint8_t grp_id;

    grp_id =  strtoul ( argv[0], NULL, 0 );
    cli_print(cli, "grp_id:%d\n", grp_id);

    iifgrp_show_one(cli, grp_id);
    return CLI_OK;
}

static int iifgrp_cmd_show_all (struct cli_def *cli, char *command, char *argv[], int argc)
{
    int i;

    for ( i = 0; i < IF_GROUP_NUM; i++ )
    {
        iifgrp_show_one(cli, i);
    }
    return CLI_OK;
}

static int iifgrp_cmd_bind_work (struct cli_def *cli, char *command, char *argv[], int argc, int flag)
{
    int rv;
    uint8_t grp_id;

    grp_id   = strtoul ( argv[0], NULL, 0 );
    cli_print(cli, "grp_id:%d, flag:%d", grp_id, flag);
    rv = md_iifgrp_bind_work(grp_id, (md_ifgrp_work_t)flag, flag);
    if ( rv )
    {
        cli_print(cli, "rv=%d\n", rv);
        return CLI_ERROR;
    }
    return CLI_OK;
}

static int iifgrp_cmd_set_priority (struct cli_def *cli, char *command, char *argv[], int argc, int flag)
{
    int rv;
    uint8_t grp_id;
    int priority;

    grp_id   = strtoul ( argv[0], NULL, 0 );
    priority = strtoul ( argv[3], NULL, 0 );
    cli_print(cli, "grp_id:%d, flag:%d, priority:%d\n", grp_id, flag, priority);
    rv = md_iifgrp_bind_work(grp_id, (md_ifgrp_work_t)flag, priority);
    if ( rv )
    {
        cli_print(cli, "rv=%d\n", rv);
        return CLI_ERROR;
    }
    return CLI_OK;
}

static int iifgrp_cmd_restore_priority (struct cli_def *cli, char *command, char *argv[], int argc)
{
    int rv;
    uint8_t grp_id;

    grp_id   = strtoul ( argv[0], NULL, 0 );
    cli_print(cli, "grp_id:%d\n", grp_id);
    rv = md_iifgrp_retore_work( grp_id );
    if ( rv )
    {
        cli_print(cli, "rv=%d\n", rv);
        return CLI_ERROR;
    }

    return CLI_OK;
}

int iifgrp_cmd_init( struct cli_def *cli)
{
    int flag_ifgrp_work = 0;

    struct cli_command *c, *c1, *c2, *c3;
    char tmpstr_ifgrp[128], tmpstr_if[128], tmpstr_priority[128];

    sprintf ( tmpstr_ifgrp, "<0-%d>", MAX_IF_GROUP_NUM - 1 );
    sprintf ( tmpstr_if,    "<0-%d>", MAX_IF_NUM - 1 );

    /* add input interface group member */
    c = cli_register_command(cli, NULL, "iifgroup", NULL, PRIVILEGE_UNPRIVILEGED,
        MODE_EXEC, "input interface group");
    c1 = cli_register_command(cli, c, tmpstr_ifgrp, NULL, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "group id");
    cli_register_command(cli, c1, tmpstr_if, iifgrp_cmd_add, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "interface id");

    /* bind rule to group */

    c2 = cli_register_command(cli, c1, "bind", NULL, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "bind");

    /* set acl work priority */
    flag_ifgrp_work = (int) MD_IF_GRP_WORK_ACL;
    cli_register_command_f(cli, c2, "acl", iifgrp_cmd_bind_work, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "acl work", flag_ifgrp_work);

    /* set sip work priority */
    flag_ifgrp_work = (int) MD_IF_GRP_WORK_IP;
    cli_register_command_f(cli, c2, "sip", iifgrp_cmd_bind_work, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "sip work", flag_ifgrp_work);

    /* set string work priority */
    flag_ifgrp_work = (int) MD_IF_GRP_WORK_STRING;
    cli_register_command_f(cli, c2, "string", iifgrp_cmd_bind_work, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "string work", flag_ifgrp_work);

    /* set payload work priority */
    flag_ifgrp_work = (int) MD_IF_GRP_WORK_PAYLOAD;
    cli_register_command_f(cli, c2, "payload", iifgrp_cmd_bind_work, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "payload work", flag_ifgrp_work);

    /* set input interface group priority */
    c2 = cli_register_command(cli, c1, "priority", NULL, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "priority");
    sprintf ( tmpstr_priority, "<0-%d>", MAX_WORK_PRIORITY - 1 );

    /* set acl work priority */
    c3 = cli_register_command(cli, c2, "acl", NULL, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "acl work");
    flag_ifgrp_work = (int) MD_IF_GRP_WORK_ACL;
    cli_register_command_f(cli, c3, tmpstr_priority, iifgrp_cmd_set_priority, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "priority degress", flag_ifgrp_work);

    /* set sip work priority */
    c3 = cli_register_command(cli, c2, "sip", NULL, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "sip work");
    flag_ifgrp_work = (int) MD_IF_GRP_WORK_IP;
    cli_register_command_f(cli, c3, tmpstr_priority, iifgrp_cmd_set_priority, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "priority degress", flag_ifgrp_work);

    /* set string work priority */
    c3 = cli_register_command(cli, c2, "string", NULL, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "string work");
    flag_ifgrp_work = (int) MD_IF_GRP_WORK_STRING;
    cli_register_command_f(cli, c3, tmpstr_priority, iifgrp_cmd_set_priority, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "priority degress", flag_ifgrp_work);

    /* set payload work priority */
    c3 = cli_register_command(cli, c2, "payload", NULL, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "payload work");
    flag_ifgrp_work = (int) MD_IF_GRP_WORK_PAYLOAD;
    cli_register_command_f(cli, c3, tmpstr_priority, iifgrp_cmd_set_priority, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "priority degress", flag_ifgrp_work);

    return CLI_OK;
}

static int iifgrp_cmd_unbind_work (struct cli_def *cli, char *command, char *argv[], int argc, int flag)
{
    int rv;
    uint8_t grp_id;

    grp_id   = strtoul ( argv[0], NULL, 0 );
    cli_print(cli, "grp_id:%d, flag:%d", grp_id, flag);
    rv = md_iifgrp_unbind_work(grp_id, (md_ifgrp_work_t)flag);
    if ( rv )
    {
        cli_print(cli, "rv=%d\n", rv);
        return CLI_ERROR;
    }
    return CLI_OK;
}

int iifgrp_cmd_del_init( struct cli_def *cli, struct cli_command *root)
{
    struct cli_command *c1, *c2, *c3;
    char tmpstr_ifgrp[128], tmpstr_if[128];
    int flag_ifgrp_work;

    sprintf ( tmpstr_ifgrp, "<0-%d>", MAX_IF_GROUP_NUM - 1 );
    sprintf ( tmpstr_if,    "<0-%d>", MAX_IF_NUM - 1 );


    /* del input interface group member */
    c1 = cli_register_command(cli, root, "iifgroup", NULL, PRIVILEGE_UNPRIVILEGED,
        MODE_EXEC, "input interface group");
    c2 = cli_register_command(cli, c1, tmpstr_ifgrp, NULL, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "group id");
    cli_register_command(cli, c2, tmpstr_if, iifgrp_cmd_del, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "interface id");

    /* del all input interface group member */
    cli_register_command(cli, c2, "all", iifgrp_cmd_del_all, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "interface id");

    /* restore default work priority */
    cli_register_command(cli, c2, "priority", iifgrp_cmd_restore_priority, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "interface id");

    /* unbind work from group */
    c3 = cli_register_command(cli, c2, "bind", iifgrp_cmd_restore_priority, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "bind");
    /* set acl work priority */
    flag_ifgrp_work = (int) MD_IF_GRP_WORK_ACL;
    cli_register_command_f(cli, c3, "acl", iifgrp_cmd_unbind_work, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "acl work", flag_ifgrp_work);

    /* set sip work priority */
    flag_ifgrp_work = (int) MD_IF_GRP_WORK_IP;
    cli_register_command_f(cli, c3, "sip", iifgrp_cmd_unbind_work, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "sip work", flag_ifgrp_work);

    /* set string work priority */
    flag_ifgrp_work = (int) MD_IF_GRP_WORK_STRING;
    cli_register_command_f(cli, c3, "string", iifgrp_cmd_unbind_work, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "string work", flag_ifgrp_work);

    /* set payload work priority */
    flag_ifgrp_work = (int) MD_IF_GRP_WORK_PAYLOAD;
    cli_register_command_f(cli, c3, "payload", iifgrp_cmd_unbind_work, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "payload work", flag_ifgrp_work);
    return CLI_OK;
}

int iifgrp_cmd_show_init( struct cli_def *cli, struct cli_command *root)
{
    struct cli_command *c1;
    char tmpstr_ifgrp[128];

    sprintf ( tmpstr_ifgrp, "<0-%d>", MAX_IF_GROUP_NUM - 1 );

    /* show input interface group member */
    c1 = cli_register_command(cli, root, "iifgroup", NULL, PRIVILEGE_UNPRIVILEGED,
        MODE_EXEC, "input interface group");
    cli_register_command(cli, c1, tmpstr_ifgrp, iifgrp_cmd_show, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "group id");
    cli_register_command(cli, c1, "all", iifgrp_cmd_show_all, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "interface id");

    return CLI_OK;
}

static int oifgrp_cmd_add (struct cli_def *cli, char *command, char *argv[], int argc)
{
    int rv;
    uint8_t grp_id, if_id;

    grp_id =  strtoul ( argv[0], NULL, 0 );
    if_id  =  strtoul ( argv[1], NULL, 0 );
    cli_print(cli, "grp_id:%d, if_id:%d\n", grp_id, if_id);
    rv = md_oifgrp_add(grp_id, if_id);
    if (rv)
    {
        cli_print(cli, "rv=%d\n", rv);
        return CLI_ERROR;
    }

    return CLI_OK;
}

static int oifgrp_cmd_del (struct cli_def *cli, char *command, char *argv[], int argc)
{
    int rv;
    uint8_t grp_id, if_id;

    grp_id =  strtoul ( argv[0], NULL, 0 );
    if_id  =  strtoul ( argv[1], NULL, 0 );
    cli_print(cli, "grp_id:%d, if_id:%d\n", grp_id, if_id);
    rv = md_oifgrp_del(grp_id, if_id);
    if (rv)
    {
        cli_print(cli, "rv=%d\n", rv);
        return CLI_ERROR;
    }
    return CLI_OK;
}

static int oifgrp_cmd_del_all (struct cli_def *cli, char *command, char *argv[], int argc)
{
    int rv;
    uint8_t grp_id;
    int i;

    grp_id =  strtoul ( argv[0], NULL, 0 );
    cli_print(cli, "grp_id:%d\n", grp_id);
    for ( i = 0; i < MAX_IF_NUM; i++ )
    {
        rv = md_oifgrp_del(grp_id, i);
        if (rv)
        {
            cli_print(cli, "rv=%d\n", rv);
        }
    }
    return CLI_OK;
}

char *oif_grp_mode_string[OIF_GRP_MODE_MAX] =
{
    "ROUND_ROBIN       ",
    "WEIGHT_ROUND_ROBIN",
    "HASH              ",
    "WEIGHT_HASH       ",
};

char *oif_grp_hash_string[OIF_GRP_HASH_MAX] =
{
    "SIP           ",
    "DIP           ",
    "USER_INTEGRITY",
    "SDIP          ",
    "FIVE_TUPLE    ",
};

void oifgrp_show_one(struct cli_def *cli, uint8_t grp_id)
{
    int i,j;
    int index,bit;
    oif_grp grp;
    oif_grp_up_port_member *up_port;

    md_oifgrp_get(grp_id, &grp);
    cli_print(cli, "input interface group %d", grp.if_grp_id);
    cli_print(cli, "%d members:", grp.port_num);
    for ( i = 0; i < MAX_IF_NUM; i++ )
    {
        index = i / UINT64_BIT_NUM;
        bit   = i % UINT64_BIT_NUM;
        if ( grp.ifmask[index] & (1 << (UINT64_BIT_NUM - bit -1)) )
        {
            cli_print(cli, "%d ", i);
        }
    }

    cli_print(cli, "up members:");
    for ( i = 0; i < MAX_IF_NUM; i++ )
    {
        index = i / UINT64_BIT_NUM;
        bit   = i % UINT64_BIT_NUM;
        if ( grp.up_ifmask[index] & (1 << (UINT64_BIT_NUM - bit -1)) )
        {
            cli_print(cli, "%d ", i);
        }
    }

    cli_print(cli, "dn members:");
    for ( i = 0; i < MAX_IF_NUM; i++ )
    {
        index = i / UINT64_BIT_NUM;
        bit   = i % UINT64_BIT_NUM;
        if ( grp.ifmask[index] & (1 << (UINT64_BIT_NUM - bit -1)) &&
              !(grp.up_ifmask[index] & (1 << (UINT64_BIT_NUM - bit -1)))  )
        {
            cli_print(cli, "%d ", i);
        }
    }

    cli_print(cli, "mode %s", oif_grp_mode_string[grp.mode]);
    cli_print(cli, "hash %s", oif_grp_hash_string[grp.hash]);
    cli_print(cli, "master %d", grp.master);

    for ( i = 0; i < 2; i++ )
    {
        up_port = &grp.up_port[i];
        cli_print(cli, "up_port[%d].up_port_num=%d", i, up_port->up_port_num);
        for ( j = 0; j < up_port->up_port_num; j++ )
        {
            cli_print(cli, "%d ", up_port->up_port[j]);
        }
    }
}

static int oifgrp_cmd_show (struct cli_def *cli, char *command, char *argv[], int argc)
{
    uint8_t grp_id;

    grp_id =  strtoul ( argv[0], NULL, 0 );
    cli_print(cli, "grp_id:%d\n", grp_id);

    oifgrp_show_one(cli, grp_id);
    return CLI_OK;
}

static int oifgrp_cmd_show_all (struct cli_def *cli, char *command, char *argv[], int argc)
{
    int i;

    for ( i = 0; i < IF_GROUP_NUM; i++ )
    {
        oifgrp_show_one(cli, i);
    }
    return CLI_OK;
}

static int oifgrp_cmd_set_mode (struct cli_def *cli, char *command, char *argv[], int argc, int flag)
{
    int rv;
    uint8_t grp_id;
    grp_id =  strtoul ( argv[0], NULL, 0 );
    cli_print(cli, "grp_id:%d\n", grp_id);
    rv = md_oifgrp_set_mode(grp_id, (oif_grp_mode)flag );
    if (rv )
    {
        cli_print(cli, "ERROR rv=%d", rv);
        return CLI_ERROR;
    }
    return CLI_OK;
}

static int oifgrp_cmd_set_hash (struct cli_def *cli, char *command, char *argv[], int argc, int flag)
{
    int rv;
    uint8_t grp_id;
    grp_id =  strtoul ( argv[0], NULL, 0 );
    cli_print(cli, "grp_id:%d\n", grp_id);
    rv = md_oifgrp_set_hash(grp_id, (oif_grp_hash)flag );
    if (rv )
    {
        cli_print(cli, "ERROR rv=%d", rv);
        return CLI_ERROR;
    }
    return CLI_OK;
}

int oifgrp_cmd_init( struct cli_def *cli)
{
    int flag_ifgrp_mode = 0;
    int flag_ifgrp_hash = 0;

    struct cli_command *c, *c1, *c2;
    char tmpstr_ifgrp[128], tmpstr_if[128];

    sprintf ( tmpstr_ifgrp, "<0-%d>", MAX_IF_GROUP_NUM - 1 );
    sprintf ( tmpstr_if,    "<0-%d>", MAX_IF_NUM - 1 );

    /* add output interface group member */
    c = cli_register_command(cli, NULL, "oifgroup", NULL, PRIVILEGE_UNPRIVILEGED,
        MODE_EXEC, "output interface group");
    c1 = cli_register_command(cli, c, tmpstr_ifgrp, NULL, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "group id");
    cli_register_command(cli, c1, tmpstr_if, oifgrp_cmd_add, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "interface id");

    /* set output interface group mode */
    c2 = cli_register_command(cli, c1, "mode", NULL, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "mode");
    /* round robin mode */
    flag_ifgrp_mode = OIF_GRP_MODE_ROUND_ROBIN;
    cli_register_command_f(cli, c2, "round-robin", oifgrp_cmd_set_mode, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "round-robin mode", flag_ifgrp_mode);

    /* weight round robin mode */
    flag_ifgrp_mode = OIF_GRP_MODE_WEIGHT_ROUND_ROBIN;
    cli_register_command_f(cli, c2, "wround-robin", oifgrp_cmd_set_mode, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "weight round-robin mode", flag_ifgrp_mode);

    /* hash mode */
    flag_ifgrp_mode = OIF_GRP_MODE_HASH;
    cli_register_command_f(cli, c2, "hash", oifgrp_cmd_set_mode, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "hash mode", flag_ifgrp_mode);

    /* weight hash mode */
    flag_ifgrp_mode = OIF_GRP_MODE_WEIGHT_HASH;
    cli_register_command_f(cli, c2, "whash", oifgrp_cmd_set_mode, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "weight hash mode", flag_ifgrp_mode);

    /* set output interface group hash */
    c2 = cli_register_command(cli, c1, "hash", NULL, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "mode");
    /* sip hash  */
    flag_ifgrp_hash = OIF_GRP_HASH_SIP;
    cli_register_command_f(cli, c2, "sip", oifgrp_cmd_set_hash, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "source ip", flag_ifgrp_hash);

    /* dip hash */
    flag_ifgrp_hash = OIF_GRP_HASH_DIP;
    cli_register_command_f(cli, c2, "dip", oifgrp_cmd_set_hash, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "destination ip", flag_ifgrp_hash);

    /* user intgrity hash */
    flag_ifgrp_hash = OIF_GRP_HASH_USER_INTEGRITY;
    cli_register_command_f(cli, c2, "user-integrity", oifgrp_cmd_set_hash, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "user integrity", flag_ifgrp_hash);
    /* sdip hash */
    flag_ifgrp_hash = OIF_GRP_HASH_SDIP;
    cli_register_command_f(cli, c2, "sdip", oifgrp_cmd_set_hash, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "source and destination ip", flag_ifgrp_hash);

    /* five-tuple hash */
    flag_ifgrp_hash = OIF_GRP_HASH_FIVE_TUPLE;
    cli_register_command_f(cli, c2, "five-tuple", oifgrp_cmd_set_hash, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "five tuple", flag_ifgrp_hash);
    return CLI_OK;
}

int oifgrp_cmd_del_init( struct cli_def *cli, struct cli_command *root)
{
    struct cli_command *c1, *c2;
    char tmpstr_ifgrp[128], tmpstr_if[128];

    sprintf ( tmpstr_ifgrp, "<0-%d>", MAX_IF_GROUP_NUM - 1 );
    sprintf ( tmpstr_if,    "<0-%d>", MAX_IF_NUM - 1 );


    /* del output interface group member */
    c1 = cli_register_command(cli, root, "oifgroup", NULL, PRIVILEGE_UNPRIVILEGED,
        MODE_EXEC, "output interface group");
    c2 = cli_register_command(cli, c1, tmpstr_ifgrp, NULL, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "group id");
    cli_register_command(cli, c2, tmpstr_if, oifgrp_cmd_del, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "interface id");

    /* del all output interface group member */
    cli_register_command(cli, c2, "all", oifgrp_cmd_del_all, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "interface id");

    return CLI_OK;
}

int oifgrp_cmd_show_init( struct cli_def *cli, struct cli_command *root)
{
    struct cli_command *c1;
    char tmpstr_ifgrp[128];

    sprintf ( tmpstr_ifgrp, "<0-%d>", MAX_IF_GROUP_NUM - 1 );

    /* show output interface group member */
    c1 = cli_register_command(cli, root, "oifgroup", NULL, PRIVILEGE_UNPRIVILEGED,
        MODE_EXEC, "output interface group");
    cli_register_command(cli, c1, tmpstr_ifgrp, oifgrp_cmd_show, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "group id");
    cli_register_command(cli, c1, "all", oifgrp_cmd_show_all, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "interface id");


    return CLI_OK;
}

void copytogrp_show_one(struct cli_def *cli, uint8_t grp_id)
{
    int i;
    int rv;
    copyto_grp grp;
    if_grp_work   *grp_work   = NULL;
    uint8_t in;

    md_cifgrp_get(grp_id, &grp);
    cli_print(cli, "input interface group %d\n", grp.if_grp_id);

    grp_work = &grp.grp_work;
    for ( i = 0; i < MAX_GRP_ACL; i++ )
    {
        if ( grp_work->priority[i] != INVALID_IFGRP_WORK_INDEX)
        {
            cli_print(cli, "priority %d, work %s", i, ifgrp_work[grp_work->priority[i]]);
        }
    }

    for ( i = 0; i < MAX_GRP_ACL; i++ )
    {
        if ( grp_work->grp_acl[grp_work->master][i] != NULL )
        {
            rv = md_if_grp_search_work_table(grp_work->grp_acl[grp_work->master][i], &in);
            if ( rv )
            {
                cli_print(cli, "ERROR rv=%d", rv);
            }
            if (in != INVALID_IFGRP_WORK_INDEX)
            {
                cli_print(cli, "%d, work %s", i, ifgrp_work[in]);
            }
        }
    }
}

static int cifgrp_cmd_show (struct cli_def *cli, char *command, char *argv[], int argc)
{
    uint8_t grp_id;

    grp_id =  strtoul ( argv[0], NULL, 0 );
    cli_print(cli, "grp_id:%d\n", grp_id);

    copytogrp_show_one(cli, grp_id);
    return CLI_OK;
}

static int cifgrp_cmd_show_all (struct cli_def *cli, char *command, char *argv[], int argc)
{
    int i;

    for ( i = 0; i < IF_GROUP_NUM; i++ )
    {
        copytogrp_show_one(cli, i);
    }
    return CLI_OK;
}

static int cifgrp_cmd_set_priority (struct cli_def *cli, char *command, char *argv[], int argc, int flag)
{
    int rv;
    uint8_t grp_id;
    int priority;

    grp_id   = strtoul ( argv[0], NULL, 0 );
    priority = strtoul ( argv[3], NULL, 0 );
    cli_print(cli, "grp_id:%d, flag:%d, priority:%d\n", grp_id, flag, priority);
    rv = md_cifgrp_bind_work(grp_id, (md_ifgrp_work_t)flag, priority);
    if ( rv )
    {
        cli_print(cli, "ERROR rv=%d\n", rv);
        return CLI_ERROR;
    }
    return CLI_OK;
}

static int cifgrp_cmd_restore_priority (struct cli_def *cli, char *command, char *argv[], int argc)
{
    int rv;
    uint8_t grp_id;

    grp_id   = strtoul ( argv[0], NULL, 0 );
    cli_print(cli, "grp_id:%d\n", grp_id);
    rv = md_cifgrp_retore_work( grp_id );
    if ( rv )
    {
        cli_print(cli, "ERROR rv=%d\n", rv);
        return CLI_ERROR;
    }
    return CLI_OK;
}

static int cifgrp_cmd_bind_work (struct cli_def *cli, char *command, char *argv[], int argc, int flag)
{
    int rv;
    uint8_t grp_id;

    grp_id   = strtoul ( argv[0], NULL, 0 );
    cli_print(cli, "grp_id:%d, flag:%d", grp_id, flag);
    rv = md_cifgrp_bind_work(grp_id, (md_ifgrp_work_t)flag, flag);
    if ( rv )
    {
        cli_print(cli, "rv=%d\n", rv);
        return CLI_ERROR;
    }
    return CLI_OK;
}

static int cifgrp_cmd_unbind_work (struct cli_def *cli, char *command, char *argv[], int argc, int flag)
{
    int rv;
    uint8_t grp_id;

    grp_id   = strtoul ( argv[0], NULL, 0 );
    cli_print(cli, "grp_id:%d, flag:%d", grp_id, flag);
    rv = md_cifgrp_unbind_work(grp_id, (md_ifgrp_work_t)flag);
    if ( rv )
    {
        cli_print(cli, "rv=%d\n", rv);
        return CLI_ERROR;
    }
    return CLI_OK;
}

int cifgrp_cmd_init( struct cli_def *cli)
{
    int flag_ifgrp_work = 0;

    struct cli_command *c, *c1, *c2, *c3;
    char tmpstr_ifgrp[128], tmpstr_if[128], tmpstr_priority[128];

    sprintf ( tmpstr_ifgrp, "<0-%d>", MAX_IF_GROUP_NUM - 1 );
    sprintf ( tmpstr_if,    "<0-%d>", MAX_IF_NUM - 1 );


    /* set copy-to interface group priority */
    c = cli_register_command(cli, NULL, "cifgroup", NULL, PRIVILEGE_UNPRIVILEGED,
        MODE_EXEC, "copy-to interface group");
    c1 = cli_register_command(cli, c, tmpstr_ifgrp, NULL, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "group id");
    c2 = cli_register_command(cli, c1, "priority", NULL, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "priority");
    sprintf ( tmpstr_priority, "<0-%d>", MAX_WORK_PRIORITY - 1 );

    /* set acl work priority */
    c3 = cli_register_command(cli, c2, "acl", NULL, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "acl work");
    flag_ifgrp_work = (int) MD_IF_GRP_WORK_ACL;
    cli_register_command_f(cli, c3, tmpstr_priority, cifgrp_cmd_set_priority, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "priority degress", flag_ifgrp_work);

    /* set sip work priority */
    c3 = cli_register_command(cli, c2, "sip", NULL, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "sip work");
    flag_ifgrp_work = (int) MD_IF_GRP_WORK_IP;
    cli_register_command_f(cli, c3, tmpstr_priority, cifgrp_cmd_set_priority, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "priority degress", flag_ifgrp_work);

    /* set string work priority */
    c3 = cli_register_command(cli, c2, "string", NULL, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "string work");
    flag_ifgrp_work = (int) MD_IF_GRP_WORK_STRING;
    cli_register_command_f(cli, c3, tmpstr_priority, cifgrp_cmd_set_priority, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "priority degress", flag_ifgrp_work);

    /* set payload work priority */
    c3 = cli_register_command(cli, c2, "payload", NULL, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "payload work");
    flag_ifgrp_work = (int) MD_IF_GRP_WORK_PAYLOAD;
    cli_register_command_f(cli, c3, tmpstr_priority, cifgrp_cmd_set_priority, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "priority degress", flag_ifgrp_work);

    /* bind rule to group */

    c2 = cli_register_command(cli, c1, "bind", NULL, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "bind");

    /* set acl work priority */
    flag_ifgrp_work = (int) MD_IF_GRP_WORK_ACL;
    cli_register_command_f(cli, c2, "acl", cifgrp_cmd_bind_work, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "acl work", flag_ifgrp_work);
    /* set sip work priority */
    flag_ifgrp_work = (int) MD_IF_GRP_WORK_IP;
    cli_register_command_f(cli, c2, "sip", cifgrp_cmd_bind_work, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "sip work", flag_ifgrp_work);
    /* set string work priority */
    flag_ifgrp_work = (int) MD_IF_GRP_WORK_STRING;
    cli_register_command_f(cli, c2, "string", cifgrp_cmd_bind_work, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "string work", flag_ifgrp_work);
    /* set payload work priority */
    flag_ifgrp_work = (int) MD_IF_GRP_WORK_PAYLOAD;
    cli_register_command_f(cli, c2, "payload", cifgrp_cmd_bind_work, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "payload work", flag_ifgrp_work);

    return CLI_OK;
}

int cifgrp_cmd_show_init( struct cli_def *cli, struct cli_command *root)
{
    struct cli_command *c1;
    char tmpstr_ifgrp[128];

    sprintf ( tmpstr_ifgrp, "<0-%d>", MAX_IF_GROUP_NUM - 1 );

    /* show copy-to interface group member */
    c1 = cli_register_command(cli, root, "cifgroup", NULL, PRIVILEGE_UNPRIVILEGED,
        MODE_EXEC, "copy-to interface group");
    cli_register_command(cli, c1, tmpstr_ifgrp, cifgrp_cmd_show, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "group id");
    cli_register_command(cli, c1, "all", cifgrp_cmd_show_all, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "interface id");

    return CLI_OK;
}

int cifgrp_cmd_del_init( struct cli_def *cli, struct cli_command *root)
{
    struct cli_command *c1, *c2, *c3;
    char tmpstr_ifgrp[128];
    int flag_ifgrp_work;

    sprintf ( tmpstr_ifgrp, "<0-%d>", MAX_IF_GROUP_NUM - 1 );

    /* restore default work priority */
    c1 = cli_register_command(cli, root, "cifgroup", NULL, PRIVILEGE_UNPRIVILEGED,
        MODE_EXEC, "copy-to interface group");
    c2 = cli_register_command(cli, c1, tmpstr_ifgrp, NULL, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "group id");
    cli_register_command(cli, c2, "priority", cifgrp_cmd_restore_priority, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "interface id");

    /* restore default work priority */
    cli_register_command(cli, c2, "priority", iifgrp_cmd_restore_priority, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "interface id");

    /* unbind work from group */
    c3 = cli_register_command(cli, c2, "bind", iifgrp_cmd_restore_priority, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "bind");
    /* set acl work priority */
    flag_ifgrp_work = (int) MD_IF_GRP_WORK_ACL;
    cli_register_command_f(cli, c3, "acl", cifgrp_cmd_unbind_work, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "acl work", flag_ifgrp_work);

    /* set sip work priority */
    flag_ifgrp_work = (int) MD_IF_GRP_WORK_IP;
    cli_register_command_f(cli, c3, "sip", cifgrp_cmd_unbind_work, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "sip work", flag_ifgrp_work);

    /* set string work priority */
    flag_ifgrp_work = (int) MD_IF_GRP_WORK_STRING;
    cli_register_command_f(cli, c3, "string", cifgrp_cmd_unbind_work, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "string work", flag_ifgrp_work);

    /* set payload work priority */
    flag_ifgrp_work = (int) MD_IF_GRP_WORK_PAYLOAD;
    cli_register_command_f(cli, c3, "payload", cifgrp_cmd_unbind_work, PRIVILEGE_UNPRIVILEGED,
            MODE_EXEC, "payload work", flag_ifgrp_work);

    return CLI_OK;
}
