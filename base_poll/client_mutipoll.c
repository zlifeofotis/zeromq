#include "zmq.h"

int main (int argc, char const *argv[]) {
	void* context = zmq_ctx_new();
	void* pull1 = zmq_socket(context, ZMQ_PULL);
	zmq_bind(pull1, "tcp://*:5050");
	void* pull2 = zmq_socket(context, ZMQ_PULL);
	zmq_bind(pull2, "tcp://*:4040");
	void* pull3 = zmq_socket(context, ZMQ_PULL);
	zmq_bind(pull3, "tcp://*:6060");
	printf("Starting...\n");
	zmq_pollitem_t polls[] = {
		{pull1, 0, ZMQ_POLLIN, 0},
		{pull2, 0, ZMQ_POLLIN, 0},
		{pull3, 0, ZMQ_POLLIN, 0}
	};
	int length = sizeof(polls) / sizeof(zmq_pollitem_t);
	for(;;) {
		zmq_msg_t msg;
		zmq_poll(polls, length, -1);
		if(polls[0].revents & ZMQ_POLLIN) {
			zmq_msg_init(&msg);
			zmq_msg_recv(&msg, pull1, 0);
			zmq_msg_close(&msg);
		}
		if(polls[1].revents > 0) {
			zmq_msg_init(&msg);
			zmq_msg_recv(&msg, pull2, 0);
			zmq_msg_close(&msg);
		}
		if(polls[2].revents > 0) {
			zmq_msg_init(&msg);
			zmq_msg_recv(&msg, pull3, 0);
			zmq_msg_close(&msg);
		} 
	}
	zmq_close(pull1);
	zmq_close(pull2);
	zmq_close(pull3);
	zmq_ctx_destroy(context);
	return 0; 
}
