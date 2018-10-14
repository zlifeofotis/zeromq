#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "zmq.h"

int main (int argc, char const *argv[]) {
	int len = 0;
	char *recv_s = NULL;
	void* context = zmq_ctx_new();
	zmq_ctx_set(context,ZMQ_IO_THREADS,50);
	zmq_ctx_set(context,ZMQ_MAX_SOCKETS,5000);
	printf("Client Starting….\n");
	void* request = zmq_socket(context, ZMQ_REQ);
	zmq_connect(request, "tcp://localhost:4040");
	int count = 0;
	for(;;) {
		zmq_msg_t req;
		zmq_msg_init_size(&req, strlen("hello"));
		memcpy(zmq_msg_data(&req), "hello", 5);
		printf("Sending: hello - %d\n", count);
		zmq_msg_send(&req, request, 0);
		zmq_msg_close(&req);
		zmq_msg_t reply;
		zmq_msg_init(&reply);
		zmq_msg_recv(&reply, request, 0);
		len = zmq_msg_size(&reply);
		recv_s = malloc(len + 1);
		bzero(recv_s,len + 1);
		memcpy(recv_s,zmq_msg_data(&reply),len);
		printf("R: %s\n", recv_s);
		free(recv_s);
		zmq_msg_close(&reply);
		count++;
	}
	zmq_close(request);
	zmq_ctx_destroy(context);
	return 0;
}
