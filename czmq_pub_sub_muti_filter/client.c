#include "czmq.h"

int main (int argc, char const *argv[]) {
	zctx_t* context = zctx_new();
	void* request = zsocket_new(context, ZMQ_SUB);
	printf("Starting client...\n");
	zsocket_connect(request, "tcp://localhost:4040");
	zsocket_set_subscribe (request, "Company10");
	signal(SIGINT,exit);
	char* env;
	char* msg;
	for(;;) {
		env = zstr_recv(request);
		msg = zstr_recv(request);
		printf("env: %s | %s\n", env, msg);
		if(!msg) 
			break;
		free(env);
		free(msg);
		zclock_sleep(1);
	}
	zsocket_destroy(context, request);
	zctx_destroy(&context);
	return 0; 
}
