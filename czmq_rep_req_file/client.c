#include <stdio.h>
#include "czmq.h"

int main (int argc, char const *argv[]) {
	zctx_t* context = zctx_new();
	void* request = zsocket_new(context, ZMQ_REQ);
	printf("Starting client...\n");
	zsocket_connect(request, "tcp://localhost:5050");
	zstr_send(request, "Hello");
	zmsg_t* msg = zmsg_recv(request);
	zfile_mkdir("test1");
	FILE* file = fopen("test1/server.txt", "w");
	zmsg_save(msg, file);
	fclose(file);
	zmsg_destroy(&msg);
	zsocket_destroy(context, request);
	zctx_destroy(&context);
	return 0; 
}
