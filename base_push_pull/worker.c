#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "zmq.h"
#define DEBUG

double square(double x) {
	return x * x;
}
double average(double x, double y) {
	return (x + y) / 2.0;
}
double good_enough(double guess, double x) {
	return abs(square(guess) - x) < 0.000001;
}
double improve(double guess, double x) {
	return average(guess, x / guess);
}
double sqrt_inner(double guess, double x) {
	if(good_enough(guess, x))
		return guess;
	else
		return sqrt_inner(improve(guess, x), x);
}
double newton_sqrt(double x) {
	return sqrt_inner(1.0, x);
}

int main (int argc, char const *argv[]) {
	void* context = zmq_ctx_new();
	// Let's initialize a socket to receive messages.
	void* receiver = zmq_socket(context, ZMQ_PULL);
	zmq_connect(receiver, "tcp://localhost:4040");
	// Let's initialize a socket to send the messages.
	void* sender = zmq_socket(context, ZMQ_PUSH);
	zmq_connect(sender, "tcp://localhost:5050");
	zmq_msg_t reply;
	zmq_msg_t message;
	char* ssend = "T";
	int t_length = strlen(ssend);
	int length;
	char* msg; 
	double val;
	int count=1;
	for(;;) {
		zmq_msg_init(&reply);
		zmq_msg_recv(&reply, receiver, 0);
		length = zmq_msg_size(&reply);
		msg = malloc(length + 1);
		bzero(msg,length + 1);
		memcpy(msg, zmq_msg_data(&reply), length);
		zmq_msg_close(&reply);
		val = atof(msg);
		printf("%.1f: %.1f : %d\n", val, newton_sqrt(val),count);
		fflush(stdout);
		free(msg);
		zmq_msg_init_size(&message, t_length);
		memcpy(zmq_msg_data(&message), ssend, t_length);
		#ifdef DEBUG
		printf("%s | %d\n",ssend,count);
		#endif
		zmq_msg_send(&message, sender, 0);
		zmq_msg_close(&message);
		count++;
		sleep(1);
	}
	zmq_close(receiver);
	zmq_close(sender);
	zmq_ctx_destroy(context);
	return 0; 
}
