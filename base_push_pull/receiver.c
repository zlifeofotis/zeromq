#include <stdlib.h>
#include <string.h>
#include "zmq.h"

int main (int argc, char const *argv[]) {
	void* context = zmq_ctx_new();
	void* receiver = zmq_socket(context, ZMQ_PULL);
	zmq_bind(receiver, "tcp://*:5050");
	// We receive the first message and discard it since it's the
	// signal start of batch which is -1.
	zmq_msg_t reply;
	zmq_msg_init(&reply);
	zmq_msg_recv(&reply, receiver, 0);
	int length = zmq_msg_size(&reply);
	char* msg = malloc(length + 1);
	bzero(msg,length + 1);
	memcpy(msg, zmq_msg_data(&reply), length);
	zmq_msg_close(&reply);
	printf("%s\n",msg);
	free(msg);
	int count = 1;
	char* value;
	for(;;) {
		zmq_msg_init(&reply);
		zmq_msg_recv(&reply, receiver, 0);
		length = zmq_msg_size(&reply);
		value = malloc(length + 1);
		bzero(value,length + 1);
		memcpy(value, zmq_msg_data(&reply), length);
		zmq_msg_close(&reply);
		printf("%s : %d\n",value,count);
		fflush(stdout);
		free(value);
		count++;
	}
	zmq_close(receiver);
	zmq_ctx_destroy(context);
	return 0; 
}
