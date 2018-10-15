#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "zmq.h"
#define DEBUG

int main (int argc, char const *argv[]) {
	void* context = zmq_ctx_new();
	void* publisher = zmq_socket(context, ZMQ_PUB);
	assert(publisher);
	printf("Starting server...\n");
	int conn = zmq_bind(publisher, "tcp://*:4040");
	assert(conn == 0);
	conn = zmq_bind(publisher, "ipc://stock.ipc");
	assert(conn == 0);
	const char* companies[3] = {"Company1", "Company10","Company101"};
	int count = 0;
	int price =0;
	int which_company = 0;
	int index = 0;
	int err_i;
	char update[64];
	#ifdef DEBUG
	int j;
	char *s;
	#endif
	for(;;) {
		price = count % 17;
		which_company = count % 3;
		sprintf(update, "%s| %d", companies[which_company], price);
		index = strlen(update);
		printf("%s ---%d\n",update,index);
		zmq_msg_t message;
		zmq_msg_init_size(&message, index);
		memcpy(zmq_msg_data(&message), update, index);
		#ifdef DEBUG
		j = zmq_msg_size(&message);
		s = malloc(j+1);
		bzero(s,j+1);
		memcpy(s,zmq_msg_data(&message),j);
		printf("SEND1: %s\n",s);
		free(s);
		#endif
		err_i = zmq_msg_send(&message, publisher, 0);
		assert(err_i > 0);
		#ifdef DEBUG
		j = zmq_msg_size(&message);
		if( j > 0){
			s = malloc(j);
			bzero(s,j);
			memcpy(s,zmq_msg_data(&message),j);
			printf("SEND2: %s ---%d\n",s,j);
			free(s);
		}
		else
			printf("size of message after sended: %d\n",j);
		#endif
		zmq_msg_close(&message);
		count++;
		sleep(1);
	}
	zmq_close(publisher);
	zmq_ctx_destroy(context);
	return 0;
}
