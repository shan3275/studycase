#include <stdlib.h>
#include <stdio.h>
#include "md_init.h"
#include "md_shm.h"

int main( int argc, char *argv[] )
{
    int rv;

    rv = md_mk_shm();
    if (rv)
    {
        printf("Mk SHM FAIL!\n");
    }

    rv = md_get_shm();
    if (rv)
    {
        printf("Get SHM FAIL!\n");
    }
    else
    {
        printf("Get SHM SUCCESS!\n");
    }

    /* init share memory */
    rv = md_init();
    if (rv)
    {
        printf("Init share memory FAIL!\n");
    }
    application_main_loop(); 
    exit(EXIT_SUCCESS);
}
