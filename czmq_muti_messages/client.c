#include "czmq.h"
#include <signal.h>

int main (int argc, char const *argv[]) {
	zctx_t* context = zctx_new();
	void* request = zsocket_new(context, ZMQ_REQ);
	printf("Starting client...\n");
	zsocket_connect(request, "tcp://localhost:5050");
	signal(SIGINT,exit);
	char* reply;
	zmsg_t* msg;
	for(;;) {
		zstr_send(request, "hello");
		msg = zmsg_recv(request);
		if(!msg) {
			break; 
		}
		printf("Received: ");
		while(reply = zmsg_popstr(msg)){
			printf("%s", reply);
			fflush(stdout);
			free(reply);
			sleep(1);
		}
		printf("\n");
		zmsg_destroy(&msg);
	}
	zsocket_destroy(context, request);
	zctx_destroy(&context);
	return 0; 
}

