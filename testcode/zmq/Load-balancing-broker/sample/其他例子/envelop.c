#include "zhelpers.h"

int main()
{
	void *context = zmq_init(1);
	void *sink = zmq_socket(context, ZMQ_ROUTER);
	zmq_bind(sink, "inproc://example");

	//第一个套接字的表示
	void * anonymous = zmq_socket(context, ZMQ_REQ);
	zmq_connect(anonymous, "inproc://example");
	s_send(anonymous, "use anony");	
	s_dump(sink);

	//第二个自己设置
	void * identified = zmq_socket(context, ZMQ_REQ);
	zmq_setsockopt(identified, ZMQ_IDENTITY, "hello", 5);
	zmq_connect(identified, "inproc://example");
	s_send(identified, "use socket");
	s_dump(sink);

	zmq_close(sink);
	zmq_close(anonymous);
	zmq_close(identified);
	zmq_term(context);
	return 0;
}
