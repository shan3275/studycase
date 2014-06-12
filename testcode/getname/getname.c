/*
*  pripds.c - Print PID and PPID
*/
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <pwd.h>

int main(void)
{
    char *login;
    struct passwd *pentry;

    /* Get the password entry for login*/
    if ((login = getlogin()) == NULL) {/* oops */
        perror("getlogin");
        exit(EXIT_FAILURE);
    }

    /* Get the passwd entry for login */
    if ((pentry = getpwnam(login)) == NULL) {
        perror("getpwnam");
        exit(EXIT_FAILURE);
    }

    /* Display the passwd entry */
    printf("login    : %s\n",login);
    printf("user name: %s\n",pentry->pw_name);
    printf("UID      : %d\n",pentry->pw_uid);
    printf("GID      : %d\n",pentry->pw_gid);
    printf("gecos    : %s\n",pentry->pw_gecos);
    printf("home dir : %s\n",pentry->pw_dir);

    exit(EXIT_SUCCESS);
}

