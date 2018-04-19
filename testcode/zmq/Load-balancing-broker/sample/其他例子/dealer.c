#include "zhelpers.h"
#include <pthread.h>

static void * worker_task_a(void* args)
{
	void *context = zmq_init(1);
	void *worker = zmq_socket(context, ZMQ_DEALER);
	zmq_setsockopt(worker, ZMQ_IDENTITY, "A", 1);
	zmq_connect(worker, "ipc://routing.ipc");
	int total = 0;
	while(1)
	{
		char * request = s_recv(worker);
		int finished = (strcmp(request, "END") == 0);
		free(request);
		if(finished)
		{
			printf("A recv %d\n", total);
			break;
		}
		total++;
	}
	zmq_close(worker);
	zmq_term(context);
	return NULL;
}

static void *worker_task_b (void *args)
{
 	void *context = zmq_init (1);
	 void *worker = zmq_socket (context, ZMQ_DEALER);
 	zmq_setsockopt (worker, ZMQ_IDENTITY, "B", 1);
 	zmq_connect (worker, "ipc://routing.ipc");
 	int total = 0;
 	while (1) {
 		// 我们只接受到消息的第二部分
 		char *request = s_recv (worker);
 		int finished = (strcmp (request, "END") == 0);
 		free (request);
 		if (finished) {
 			printf ("B received: %d\n", total);
 			break;
 		}
 		total++;
 	}
 	zmq_close (worker);
 	zmq_term (context);
 	return NULL;
}

int main()
{
	void * context = zmq_init(1);
	void *client = zmq_socket(context, ZMQ_ROUTER);
	zmq_bind(client, "ipc://routing.ipc");
	
	pthread_t worker;
	pthread_create(&worker, NULL, worker_task_a, NULL);
	pthread_create (&worker, NULL, worker_task_b, NULL);

	sleep(1);
	int task_nbr;
	srandom ((unsigned) time (NULL));
	for(task_nbr = 0; task_nbr < 10; task_nbr++)
	{
		if(randof(3) > 0)
			s_sendmore(client, "A");
		else
			s_sendmore(client, "B");		

		s_send(client, "this is worker load");
	}
	s_sendmore(client, "A");
	s_send(client, "END");

	s_sendmore (client, "B");
	s_send(client, "END");
	
	zmq_close(client);
	zmq_term(context);
	return 0;
}
