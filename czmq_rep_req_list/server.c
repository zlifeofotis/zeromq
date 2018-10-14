#include "czmq.h"
#include <signal.h>

int main (int argc, char const *argv[]) {
	zctx_t* context = zctx_new();
	void* socket = zsocket_new(context, ZMQ_REP);
	zsocket_bind(socket, "tcp://*:5050");
	signal(SIGINT, exit);
	printf("Starting server...\n");
	zhash_t* map = zhash_new();
	zhash_insert(map, "user_id", "1234");
	zhash_insert(map, "user_email", "name@example.org");
	char* msg;
	char* email;
	for(;;) {
		msg = zstr_recv(socket);
		printf("Received: %s\n", msg);
		email = zhash_lookup(map, msg);
		if(email) {
			zstr_send(socket, email);
		} 
		else {
			zstr_send(socket, "Not Found");
		}
		free(msg); 
	}
	zsocket_destroy(context, socket);
	zctx_destroy(&context);
	return 0; 
}
