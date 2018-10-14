#include "czmq.h"

int main (int argc, char const *argv[]) {
	zctx_t* context = zctx_new();
	void* socket = zsocket_new(context, ZMQ_REP);
	zsocket_bind(socket, "tcp://*:5050");
	printf("Starting server...\n");
	zmsg_t* msg;
	char* rcv_s;
	for(;;) {
		rcv_s = zstr_recv(socket);	
		if(!rcv_s) {
			break;
		}
		printf("Received: %s\n", rcv_s);
		free(rcv_s);
		msg = zmsg_new();
		zmsg_addmem(msg, "w", 1);
		zmsg_addmem(msg, "o", 1);
		zmsg_addmem(msg, "r", 1);
		zmsg_addmem(msg, "l", 1);
		zmsg_addmem(msg, "d", 1);
		zmsg_send(&msg, socket);
		zmsg_destroy(&msg);
	}
	zsocket_destroy(context, socket);
	zctx_destroy(&context);
	return 0; 
}
