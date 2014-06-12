/*
 *  abort.c - Demonstrate the abort system call
 */

#include <stdlib.h>
#include <stdio.h>

int main(void)
{
    abort();
    /* Shouldn't get here */
    printf("hello\n");
    exit(EXIT_SUCCESS);
}

