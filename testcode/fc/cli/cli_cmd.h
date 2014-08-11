#ifndef __CLI_CMD_INCLUDE__
#define __CLI_CMD_INCLUDE__

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "libcli.h"

#define CLITEST_PORT                8000
#define MODE_CONFIG_INT             10

#ifdef __GNUC__
# define UNUSED(d) d __attribute__ ((unused))
#else
# define UNUSED(d) d
#endif

void cmd_init( struct cli_def *cli);
int loop_server(void );
int loop_if_monitor( void );
#endif /* end of __CLI_CMD_INCLUDE__ */
