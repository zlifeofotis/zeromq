#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include "zmq.h"
#define DEBUG


int main (int argc, char const *argv[]) {
	void* context = zmq_ctx_new();
	// This is the socket that we send messages.
	void* socket = zmq_socket(context, ZMQ_PUSH);
	zmq_bind(socket, "tcp://*:4040");
	// This is the socket that we send batch message.
	void* connector = zmq_socket(context, ZMQ_PUSH);
	zmq_connect(connector, "tcp://localhost:5050");
	printf("Please press enter when workers are ready...");
	getchar();
	printf("Sending tasks to workers...\n");
	// The first message. It's also the signal start of batch.
	int length = strlen("-1");
	zmq_msg_t message;
	zmq_msg_init_size(&message, length);
	memcpy(zmq_msg_data(&message), "-1", length);
	zmq_msg_send(&message, connector, 0);
	zmq_msg_close(&message);
	// Generate some random numbers.
	srandom((unsigned) time(NULL));
	// Send the tasks.
	int count;
	int msec = 0;
	int load;
	char string[10];
	for(count = 0; count < 100; count++) {
		load = (int) ((double) (100) * random () / RAND_MAX);
		msec += load;
		sprintf(string, "%d", load);
		length = strlen(string);
		zmq_msg_init_size(&message,length);
		memcpy(zmq_msg_data(&message),string,length);
		#ifdef DEBUG
		printf("%d | %d\n",load,count);
		#endif
		zmq_msg_send(&message,socket,0);
		zmq_msg_close(&message);
	}
	printf("Total: %d msec\n", msec);
	//sleep(1);
	zmq_close(connector);
	zmq_close(socket);
	zmq_ctx_destroy(context);
	return 0;
}
