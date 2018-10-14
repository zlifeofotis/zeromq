#include "czmq.h"

int main (int argc, char const *argv[]) {
	zctx_t* context = zctx_new();
	void* pub = zsocket_new(context, ZMQ_PUB);
	zsocket_bind(pub, "tcp://*:4040");
	printf("starting server\n");
	signal(SIGINT,exit);
	for(;;) {
		zstr_sendm(pub, "Company1");
		zstr_send(pub, "Company Message to be ignored.");
		zstr_sendm(pub, "Company10");
		zstr_send(pub, "Company message to receive.");
		zclock_sleep(1000);
	}
	zsocket_destroy(context, pub);
	zctx_destroy(&context);
	return 0; 
}
