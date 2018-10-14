#include "czmq.h"
#include <signal.h>
#include <stdlib.h>

int do_something(zloop_t* loop, zmq_pollitem_t* item, void* socket) {
	char* s = zstr_recv(socket);
	printf("%s\n", s);
	free(s);
	return 0;
}
int do_timeout(zloop_t* loop, int timer_id, void* socket) {
	printf("--- %d ---\n",timer_id);
}
int main (int argc, char const *argv[]) {
	zctx_t* context = zctx_new();
	void* socket = zsocket_new(context, ZMQ_PULL);
	zsocket_bind(socket, "tcp://*:5050");
	signal(SIGINT, exit);
	printf("Starting server...\n");
	zloop_t* loop = zloop_new();
	zloop_set_verbose(loop, 1);
	zloop_timer(loop, 1000, 0, do_timeout, NULL);
	zmq_pollitem_t poll = {socket, 0, ZMQ_POLLIN};
	zloop_poller(loop, &poll, do_something, socket);
	zloop_start(loop);
	zloop_destroy(&loop);
	zsocket_destroy(context, socket);
	zctx_destroy(&context);
	return 0; 
}
