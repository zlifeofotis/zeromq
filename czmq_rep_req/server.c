#include <string.h>
#include <stdio.h>
#include "czmq.h"

int main (int argc, char const *argv[]) {
	zctx_t* context = zctx_new();
	void* socket = zsocket_new(context, ZMQ_REP);
	zsocket_bind(socket, "tcp://*:5050");
	printf("Starting server...\n");
	char *msg;
	for(;;) {
		msg = zstr_recv(socket);
		if(!msg) {
			break;
		}
		printf("Received: %s\n", msg);
		zstr_send(socket, "world");
		free(msg);
	}
	zsocket_destroy(context, socket);
	zctx_destroy(&context);
	return 0;
}
