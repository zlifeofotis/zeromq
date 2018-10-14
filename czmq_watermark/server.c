#include "czmq.h"

int main (int argc, char const *argv[]) {
	zctx_t* context = zctx_new();
	void* socket = zsocket_new(context, ZMQ_REP);
	zsocket_bind(socket, "tcp://*:5050");
	printf("Starting server...\n");
	signal(SIGINT,exit);
	zsocket_set_hwm(socket,10);
	zmsg_t* msg;
	for(;;) {
		msg = zmsg_new();
		zmsg_addmem(msg, "h", 1);
		zmsg_addmem(msg, "e", 1);
		zmsg_addmem(msg, "l", 1);
		zmsg_addmem(msg, "l", 1);
		zmsg_addmem(msg, "o", 1);
		zmsg_send(&msg, socket);
		zmsg_destroy(&msg);
	}
	zsocket_destroy(context, socket);
	zctx_destroy(&context);
	return 0; 
}
