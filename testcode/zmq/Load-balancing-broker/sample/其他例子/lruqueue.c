#include "zhelpers.h"
#include <pthread.h>

#define NBR_CLIENTS 10
#define NBR_WORKERS 3

#define DEQUEUE(q) memmove(&(q)[0], &(q)[1], sizeof (q) - sizeof (q[0]))

static void* client_task(void *args)
{
	void *context = zmq_init(1);
	void *client = zmq_socket(context, ZMQ_REQ);
	s_set_id(client);
	zmq_connect(client, "ipc://frontend.ipc");

	s_send(client, "HELLO");
	char* reply = s_recv(client);
	printf ("Client: %s\n", reply);
	free(reply);
	zmq_close(client);
	zmq_term(context);
	return NULL;
}

static void* worker_task(void * args)
{
	void * context = zmq_init(1);
	void * worker = zmq_socket(context, ZMQ_REQ);
	s_set_id(worker);
	zmq_connect(worker, "ipc://backend.ipc");
	
	s_send(worker, "READY");
	while (1)
	{
		char* address = s_recv(worker);
		char *empty = s_recv(worker);
		free(empty);
		char* request = s_recv(worker);
		printf ("Worker: %s\n", request);
		free(request);
		
		s_sendmore(worker, address);
		s_sendmore(worker, "");
		s_send(worker, "OK");
		free(address);
	}
	zmq_close(worker);
	zmq_term(context);
	return NULL;
}


int main()
{
	void* context = zmq_init(1);
	void* frontend = zmq_socket(context, ZMQ_ROUTER);
	void *backend = zmq_socket (context, ZMQ_ROUTER);
 	zmq_bind (frontend, "ipc://frontend.ipc");
 	zmq_bind (backend,  "ipc://backend.ipc");
 	int client_nbr;
 	for (client_nbr = 0; client_nbr < NBR_CLIENTS; client_nbr++) {
 		pthread_t client;
 		pthread_create (&client, NULL, client_task, NULL);
 	}
 	int worker_nbr;
 	for (worker_nbr = 0; worker_nbr < NBR_WORKERS; worker_nbr++) {
 		pthread_t worker;
 		pthread_create (&worker, NULL, worker_task, NULL);
 	}
 	// LRU 逻辑
 	// - 一直从 backend 中获取消息；当有超过一个 worker 空闲时才从frontend 获取消息。
 	// - 当 woker 回应时，会将该 worker 标记为已准备好，并转发 woker 的回应给 client
 	// - 如果 client 发送了请求，就将该请求转发给下一个 worker
 	// 存放可用 worker 的队列
	int available_workers = 0;
 	char *worker_queue [10];	
	while (1) {
 		zmq_pollitem_t items [] = {
 		{ backend,  0, ZMQ_POLLIN, 0 },
 		{ frontend, 0, ZMQ_POLLIN, 0 }};
 		zmq_poll (items, available_workers? 2: 1, -1);
 		// 处理 backend 中 worker 的队列
 		if (items [0]. revents & ZMQ_POLLIN) {
			 // 将 worker 的地址入队
 			char *worker_addr = s_recv (backend);
 			assert (available_workers < NBR_WORKERS);
 			worker_queue [available_workers++] = worker_addr;
 			// 跳过空帧
  			char *empty = s_recv (backend);
 			assert (empty [0] == 0);
 			free (empty);
 			// 第三帧是“READY” 或是一个 client 的地址
 			char *client_addr = s_recv (backend);
 			// 如果是一个应答消息，则转发给 client
 			if (strcmp (client_addr, "READY") != 0) {
 				empty = s_recv (backend);
 				assert (empty [0] == 0);
 				free (empty);
 				char *reply = s_recv (backend);
 				s_sendmore (frontend, client_addr);
  				s_sendmore (frontend, "");
 				s_send (frontend, reply);
 				free (reply);
 				if (--client_nbr == 0)
 					break;  // 处理 N 条消息后退出
 			}
 			free (client_addr);
 		}
 		if (items [1]. revents & ZMQ_POLLIN) {
 			// 获取下一个 client 的请求，交给空闲的 worker 处理
 			// client 请求的消息格式是： [client 地址][空帧][请求内容]
 			char *client_addr = s_recv (frontend);
 			char *empty = s_recv (frontend);
			assert (empty [0] == 0);
 			free (empty);
 			char *request = s_recv (frontend);
 			s_sendmore (backend, worker_queue [0]);
 			s_sendmore (backend, "");
 			s_sendmore (backend, client_addr);
 			s_sendmore (backend, "");
 			s_send (backend, request);
 			free (client_addr);
 			free (request);
 			// 将该 worker 的地址出队
 			free (worker_queue [0]);
 			DEQUEUE (worker_queue);
 			available_workers--;
 		}
 	}
 	zmq_close (frontend);
 	zmq_close (backend);
 	zmq_term (context);
 	return 0;
}
