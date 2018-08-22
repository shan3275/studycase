#define ZMQ_BUILD_DRAFT_API 1
#include <stdio.h>
#include <zmq.h>
#include <string.h>
#include <unistd.h>

int main (int argc , char* argv[])
{
    void *context = zmq_ctx_new ();
    void *publisher = zmq_socket (context, ZMQ_RADIO);
    zmq_connect (publisher, argv[1]);
    printf("Broadcasting on radio channel 'foo'!\n");
    while (1) {
        printf("send hello\n");
        usleep(2000000);
       zmq_msg_t msg;
        zmq_msg_init_size(&msg, strlen("hello"));
        memcpy(zmq_msg_data(&msg), "hello", strlen("hello"));
        zmq_msg_set_group(&msg, "foo");
        zmq_msg_send(&msg, publisher, 0);
    }
}

