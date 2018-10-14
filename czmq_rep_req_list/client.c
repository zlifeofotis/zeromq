#include "czmq.h"
#include <signal.h>

int main (int argc, char const *argv[]) {
	zctx_t* context = zctx_new();
	void* request = zsocket_new(context, ZMQ_REQ);
	printf("Starting client...\n");
	zsocket_connect(request, "tcp://localhost:5050");
	zlist_t* list = zlist_new();
	zstr_send(request, "user_email");
	char* reply1 = zstr_recv(request);
	/*
	 * zlist_push将第2个参数push到list中,因此第2个参数代表的地址不能free
	 */
	zlist_push(list, reply1);
	zstr_send(request, "user_id");
	char* reply2 = zstr_recv(request);
	zlist_push(list, reply2);
	int length = zlist_size(list);
	char* s;
	int i;
	for(i = 0; i < length; i++) {
		s = (char *)zlist_pop(list);
		printf("%s ", s);
	}
	printf("\nfileshed\n");
	free(reply1);
	free(reply2);
	zsocket_destroy(context, request);
	zctx_destroy(&context);
	return 0; 
}
