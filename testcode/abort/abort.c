/*
*  abort.c - Demonstrate the abort system call
*/

#include <stdlib.h>
#include <stdio.h>


int main(void)
{
    abort();
    /* shouldn't get here */
    printf("Shouldn't get here!\n");
    exit(EXIT_SUCCESS);
}

