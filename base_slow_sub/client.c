#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include "zmq.h"
#define DELAY 4

int main (int argc, char const *argv[]) {
	void* context = zmq_ctx_new();
	void* subscriber = zmq_socket(context, ZMQ_SUB);
	printf("Getting data...\n");
	int conn = zmq_connect(subscriber, "tcp://localhost:4040");
	conn = zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, 0, 0);
	int i;
	time_t current_time;
	zmq_msg_t reply;
	int length;
	char* value;
	unsigned long t_timer;
	int res;
	for(i = 0; i <  10; i++) {
		current_time = time(NULL) % 86400;
		zmq_msg_init(&reply);
		zmq_msg_recv(&reply, subscriber, 0);
		length = zmq_msg_size(&reply);
		value = malloc(length + 1);
		bzero(value,length + 1);
		memcpy(value, zmq_msg_data(&reply), length);
		zmq_msg_close(&reply);
		sscanf(value, "%lu", &t_timer);
		printf("%d --- %s\n",current_time,value);
		res = abs(current_time - t_timer);
		free(value);
		/*
		if(res > DELAY) {
			printf("Subscriber is too slow. Aborting.\n");
			break;
		}
		*/
		sleep(3); 
	}
	zmq_close(subscriber);
	zmq_ctx_destroy(context);
	return 0;
}
