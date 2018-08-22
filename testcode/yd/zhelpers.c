/*  =====================================================================
    zhelpers.h

    Helper header file for example applications.
    =====================================================================
*/

//  Include a bunch of headers that we will need in the examples

#include <zmq.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//  Receive 0MQ string from socket and convert into C string
//  Caller must free returned string. Returns NULL if the context
//  is being terminated.
char * s_recv (void *socket, int *nsize) 
{
	zmq_msg_t message;
	//zmq_msg_init_size(&message, 512);
	zmq_msg_init(&message);
	if (zmq_recvmsg (socket, &message, 1) > 0)
	{
		int size = zmq_msg_size (&message);
		char *string = (char*)malloc (size + 1);
		memcpy (string, zmq_msg_data (&message), size);
		zmq_msg_close (&message);
		string [size] = 0;
		*nsize = size;
		return (string);
	}

	return NULL;
}

//  Convert C string to 0MQ string and send to socket
int s_send (void *socket, char *string) 
{
	if(NULL == string)
	{
		return -1;
	}
	int rc = zmq_send(socket, string, strlen(string), ZMQ_DONTWAIT);
	return (rc);
}

