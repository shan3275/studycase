/**************************************************************
* Copyright (C) 2014-2018 All rights reserved.
* @Version: 1.0
* @Created: 2018-04-02 21:54
* @Author: SamLiu - shan3275@gmail.com
* @Description: 
*
* @History: 
**************************************************************/

#include "zhelpers.h"

int main (void)
{
    //  Socket to receive messages on
    void *context = zmq_ctx_new ();
    //  Socket to send messages to
    void *sender = zmq_socket (context, ZMQ_PUSH);
    zmq_connect (sender, "tcp://localhost:5558");

    //  Process tasks forever
    char msg[10] = "hello ser";
    int rv;
    for(int i = 0; i < 10000000; i++)
    {
        usleep(1000);
        printf(".");
        rv = s_send (sender, msg);        //  Send results to sink
        if ( rv < 0)
        {
            printf("errno is: %d\n",errno);
            perror("errno");
        }
    }
    zmq_close (sender);
    zmq_ctx_destroy (context);
    return 0;
}
