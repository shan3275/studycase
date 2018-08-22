/*  =====================================================================
    zhelpers.h

    Helper header file for example applications.
    =====================================================================
*/

#ifndef __ZHELPERS_H_INCLUDED__
#define __ZHELPERS_H_INCLUDED__

//  Include a bunch of headers that we will need in the examples

char * s_recv (void *socket, int *nsize); 
//  Convert C string to 0MQ string and send to socket
int s_send (void *socket, char *string) ;

#endif  //  __ZHELPERS_H_INCLUDED__
