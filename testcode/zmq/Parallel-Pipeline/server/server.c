/**************************************************************
* Copyright (C) 2014-2018 All rights reserved.
* @Version: 1.0
* @Created: 2018-04-02 21:53
* @Author: SamLiu - shan3275@gmail.com
* @Description: 
*
* @History: 
**************************************************************/
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <zmq.h>

int main(){
    void *context = zmq_ctx_new();
    //void * context = zmq_init(1);

    //void *responder = zmq_socket(context, ZMQ_REP);
    void *responder = zmq_socket(context, ZMQ_PULL);
    int rc = zmq_bind(responder, "tcp://*:5558");
    assert(rc == 0);

    while(1){
    char buf[10];
    memset(buf, 0, 10);
    zmq_recv(responder, buf, 10, 0);
    printf("responder recv: %s\n", buf);
    //sleep(1);

    //zmq_send(responder, "s:reply", 7, 0);

    }
    return 0;
}
