#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "zmq.h"

int main (int argc, char const *argv[]) {
	int len = 0;
	char *recv_s = NULL;
	void* context = zmq_ctx_new();
	zmq_ctx_set(context,ZMQ_IO_THREADS,100);
	zmq_ctx_set(context,ZMQ_MAX_SOCKETS,5000);
	void* respond = zmq_socket(context, ZMQ_REP);
	zmq_bind(respond, "tcp://*:4040");
	printf("Starting…\n");
	for(;;) {
		zmq_msg_t request;
		zmq_msg_init(&request);
		zmq_msg_recv(&request, respond, 0);
		len = zmq_msg_size(&request);
		recv_s = malloc(len + 1);
		bzero(recv_s,len + 1);
		memcpy(recv_s,zmq_msg_data(&request),len);
		printf("R: %s\n", recv_s);
		free(recv_s);
		zmq_msg_close(&request);
		sleep(1); 
		zmq_msg_t reply;
		zmq_msg_init_size(&reply, strlen("world"));
		memcpy(zmq_msg_data(&reply), "world", 5);
		zmq_msg_send(&reply, respond, 0);
		zmq_msg_close(&reply);
	}
	zmq_close(respond);
	zmq_ctx_destroy(context);
	return 0;
}
