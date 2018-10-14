#include "czmq.h"
#include <signal.h>

int main (int argc, char const *argv[]) {
	zctx_t* context = zctx_new();
	void* request = zsocket_new(context, ZMQ_PUSH);
	signal(SIGINT, exit);
	printf("Starting client...\n");
	zsocket_connect(request, "tcp://localhost:5050");
	for(;;) {
		zstr_send(request, "hello");
		printf("Pushing Hello\n");
	}
	return 0; 
}
