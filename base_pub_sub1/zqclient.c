#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "zmq.h"

int main (int argc, char const *argv[]) {
	int length = 0;
	char *value = NULL;
	void* context = zmq_ctx_new();
	void* subscriber = zmq_socket(context, ZMQ_SUB);
	assert(subscriber);
	printf("Collecting stock information from the server.\n");
	int conn = zmq_connect(subscriber, "tcp://localhost:4040");
	assert(conn == 0);
	conn = zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, 0, 0);
	assert(conn == 0);
	int i;
	int err_i;
	for(i = 0; i < 10; i++) {
		zmq_msg_t reply;
		zmq_msg_init(&reply);
		err_i = zmq_msg_recv(&reply, subscriber, 0);
		assert(err_i > 0);
		length = zmq_msg_size(&reply);
		value = malloc(length + 1);
		bzero(value,length + 1);
		memcpy(value, zmq_msg_data(&reply), length);
		printf("%s\n", value);
		free(value);
		zmq_msg_close(&reply);
	}
	zmq_close(subscriber);
	zmq_ctx_destroy(context);
	return 0; 
}
