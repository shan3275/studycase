#include "cli_cmd.h"
#ifndef __IFGRP_CLI_INCLUDE__
#define __IFGRP_CLI_INCLUDE__

int iifgrp_cmd_init( struct cli_def *cli);
int iifgrp_cmd_del_init( struct cli_def *cli, struct cli_command *root);
int iifgrp_cmd_show_init( struct cli_def *cli, struct cli_command *root);
int oifgrp_cmd_init( struct cli_def *cli);
int oifgrp_cmd_del_init( struct cli_def *cli, struct cli_command *root);
int oifgrp_cmd_show_init( struct cli_def *cli, struct cli_command *root);
int cifgrp_cmd_init( struct cli_def *cli);
int cifgrp_cmd_show_init( struct cli_def *cli, struct cli_command *root);
int cifgrp_cmd_del_init( struct cli_def *cli, struct cli_command *root);
#endif /* end of __IFGRP_CLI_INCLUDE__ */
