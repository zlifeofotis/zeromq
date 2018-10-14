#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "zmq.h"

int main (int argc, char const *argv[]) {
	void* context = zmq_ctx_new();
	void* respond = zmq_socket(context, ZMQ_REP);
	zmq_bind(respond, "tcp://*:4040");
	printf("Starting...\n");
	zmq_msg_t request;
	zmq_msg_t msg1, msg2, msg3, msg4, msg5;
	for(;;) {
		zmq_msg_init(&request);
		zmq_msg_recv(&request, respond, 0);
		printf("Received: hello\n");
		zmq_msg_close(&request);
		sleep(1);
		zmq_msg_init_size(&msg1, 2);
		zmq_msg_init_size(&msg2, 2);
		zmq_msg_init_size(&msg3, 2);
		zmq_msg_init_size(&msg4, 2);
		zmq_msg_init_size(&msg5, 2);
		memcpy(zmq_msg_data(&msg1), "w", 2);
		zmq_msg_send(&msg1, respond, ZMQ_SNDMORE);
		memcpy(zmq_msg_data(&msg2), "o", 2);
		zmq_msg_send(&msg2, respond, ZMQ_SNDMORE);
		memcpy(zmq_msg_data(&msg3), "r", 2);
		zmq_msg_send(&msg3, respond, ZMQ_SNDMORE);
		memcpy(zmq_msg_data(&msg4), "l", 2);
		zmq_msg_send(&msg4, respond, ZMQ_SNDMORE);
		memcpy(zmq_msg_data(&msg5), "d", 2);
		zmq_msg_send(&msg5, respond, 0);
		zmq_msg_close(&msg1);
		zmq_msg_close(&msg2);
		zmq_msg_close(&msg3);
		zmq_msg_close(&msg4);
		zmq_msg_close(&msg5);
	}
	zmq_close(respond);
	zmq_ctx_destroy(context);
	return 0; 
}

