#include <string.h>
#include "zmq.h"

int main (int argc, char const *argv[]) {
	void* ctx = zmq_ctx_new();
	void* pull = zmq_socket(ctx, ZMQ_PULL);
	zmq_connect(pull, "tcp://localhost:4040");
	void* subscriber = zmq_socket(ctx, ZMQ_SUB);
	char* company_name = "Company1";
	int length = strlen(company_name) + 1;
	zmq_connect(subscriber, "tcp://localhost:5050");
	zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, company_name, length);
	printf("starting...\n");
	zmq_pollitem_t polls[2];
	polls[0].socket = pull;
	polls[0].fd = 0;
	polls[0].events = ZMQ_POLLIN;
	polls[0].revents = 0;
	polls[1].socket = subscriber;
	polls[1].fd = 0;
	polls[1].events = ZMQ_POLLIN;
	polls[1].revents = 0;
	for(;;) {
		zmq_msg_t msg;
		int res =  zmq_poll(polls, 2, -1);
		if(polls[0].revents > 0) {
			zmq_msg_init(&msg);
			zmq_msg_recv(&msg, pull, 0);
			zmq_msg_close(&msg);
		}
		if(polls[1].revents > 0) {
			zmq_msg_init(&msg);
			zmq_msg_recv(&msg, subscriber, 0);
			zmq_msg_close(&msg);
		}
	}
	zmq_close(pull);
	zmq_close(subscriber);
	zmq_ctx_destroy(ctx);
	return 0; 
}
