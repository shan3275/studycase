#define ZMQ_BUILD_DRAFT_API 1
#include <stdio.h>
#include <zmq.h>
#include <string.h>
#include <unistd.h>


int main (int argc, char* argv[])
{
    void *context = zmq_ctx_new ();
    void *subscriber = zmq_socket (context, ZMQ_DISH);
    int total = 0, tsize = 0;
    zmq_bind(subscriber, argv[1]);
    zmq_join(subscriber, "foo");

    zmq_msg_t msg;
    zmq_msg_init(&msg);
    usleep(1000);
    fprintf(stderr,"Pointing the dish to channel 'foo'\n");
    while(1) {
    total = 0;
    while (total < 1000000) {
        zmq_msg_recv(&msg, subscriber, 0);
        total++;
        tsize += zmq_msg_size(&msg);
        printf("total: %d,rcv:%s\n", total, zmq_msg_data(&msg));
     }
    }
    zmq_close (subscriber);
    zmq_ctx_destroy (context);
    return 0;
}
