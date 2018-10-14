#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "zmq.h"

int main (int argc, char const *argv[]) {
	void* context = zmq_ctx_new();
	void* publisher = zmq_socket(context, ZMQ_PUB);
	assert(publisher);
	printf("Starting server...\n");
	int conn = zmq_bind(publisher, "tcp://*:4040");
	assert(conn == 0);
	const char* companies[3] = {"Company1", "Company10","Company101"};
	int count = 0;
	int price =0;
	int which_company = 0;
	int index = 0;
	int err_i;
	char update[12];
	for(;;) {
		price = count % 17;
		which_company = count % 3;
		index = strlen(companies[which_company]);
		snprintf(update, sizeof(update), "%s", companies[which_company]);
		printf("%s\n",update);
		zmq_msg_t message;
		zmq_msg_init_size(&message, index);
		memcpy(zmq_msg_data(&message), update, index);
/*
		int j = zmq_msg_size(&message);
		char *s = malloc(j+1);
		bzero(s,j+1);
		memcpy(s,zmq_msg_data(&message),j);
		printf("SEND1: %s\n",s);
		free(s);
*/
		err_i = zmq_msg_send(&message, publisher, 0);
		assert(err_i > 0);
/*
		int j = zmq_msg_size(&message);
		char *s = malloc(j);
		bzero(s,j);
		memcpy(s,zmq_msg_data(&message),j);
		printf("SEND2: %s\n",s);
		free(s);
*/
		zmq_msg_close(&message);
		count++;
		sleep(1);
	}
	zmq_close(publisher);
	zmq_ctx_destroy(context);
	return 0;
}
