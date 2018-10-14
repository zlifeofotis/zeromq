#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "zmq.h"

int main (int argc, char const *argv[]) {
	void* context = zmq_ctx_new();
	printf("Client Starting....\n");
	void* request = zmq_socket(context, ZMQ_REQ);
	zmq_connect(request, "tcp://localhost:4040");
	zmq_msg_t reply;
	zmq_msg_t req;
	uint64_t more_part;
	size_t more_size = sizeof(more_part);
	for(;;){
		zmq_msg_init_size(&req, strlen("hello"));
		memcpy(zmq_msg_data(&req), "hello", 5);
		printf("Sending: hello\n");
		zmq_msg_send(&req, request, 0);
		zmq_msg_close(&req);
		for(;;) {
			zmq_msg_init(&reply);
			zmq_msg_recv(&reply, request, 0);
			printf("Received: %s\n", (char *) zmq_msg_data(&reply));
			zmq_msg_close(&reply);
			sleep(1);
			zmq_getsockopt(request, ZMQ_RCVMORE, &more_part, &more_size);
			if (!more_part)
				break;
		}
	}
	zmq_close(request);
	zmq_ctx_destroy(context);
	return 0; 
}
