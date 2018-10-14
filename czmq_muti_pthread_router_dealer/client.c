#include "czmq.h"
#include <signal.h>
#define DEBUG

int main (int argc, char const *argv[]) {
	zctx_t* context = zctx_new();
	void* request = zsocket_new(context, ZMQ_REQ);
	printf("Starting client...\n");
	zsocket_connect(request, "tcp://localhost:5050");
	signal(SIGINT,exit);
	char* reply;
	for(;;) {
		zstr_send(request, "hello");
		#ifdef DEBUG
		printf("sending: %s\n","hello");
		puts("now: zstr_recv");
		#endif
		reply = zstr_recv(request);
		if(!reply) {
			break; 
		}
		printf("Received: %s\n", reply);
		free(reply);
		sleep(1);
	}
	zsocket_destroy(context, request);
	zctx_destroy(&context);
	return 0; 
}
