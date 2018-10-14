#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include "zmq.h"

int main (int argc, char const *argv[]) {
	void* context = zmq_ctx_new();
	void* publisher = zmq_socket(context, ZMQ_PUB);
	printf("Starting Server...\n");
	zmq_bind(publisher, "tcp://*:4040");
	time_t current_time;
	char str[16];
	int s_len;
	zmq_msg_t message;
	for(;;) {
		current_time = time(NULL) % 86400;
		snprintf(str, sizeof(str), "%lu", current_time);
		s_len = strlen(str);
		printf("%s\n",str);
		zmq_msg_init_size(&message, s_len);
		memcpy(zmq_msg_data(&message), str, s_len);
		zmq_msg_send(&message, publisher, 0);
		zmq_msg_close(&message);
		sleep(1);
	}
	zmq_close(publisher);
	zmq_ctx_destroy(context);
	return 0; 
}
