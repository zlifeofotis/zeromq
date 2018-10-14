#include "czmq.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define DEBUG

void* worker(void* ctx) {
	zctx_t* context = ctx;
	void* receiver = zsocket_new(context, ZMQ_REP);
	zsocket_connect(receiver, "inproc://workers");
	char* str = NULL;
	pthread_t id = pthread_self();
	for(;;) {
		#ifdef DEBUG
		printf("now: zstr_recv | pthread id: %lu\n", id);
		#endif
		str = zstr_recv(receiver);
		#ifdef DEBUG
		printf("received: %s | pthread id: %lu\n",str, id);
		#endif 
		if(str){
			printf("Received: %s\n", str);
			free(str);
			//zclock_sleep(10);
			zstr_send(receiver, "world");
			#ifdef DEBUG
			printf("sending: %s | pthread id: %lu\n","world", id);
			#endif
		}
	}
	zsocket_destroy(context, receiver);
	return NULL;
}

int main (int argc, char const *argv[]) {
	zctx_t* context = zctx_new();
	void* clients = zsocket_new(context, ZMQ_ROUTER);
	zsocket_bind(clients, "tcp://*:5050");
	void* workers = zsocket_new(context, ZMQ_DEALER);
	zsocket_bind(workers, "inproc://workers");
	signal(SIGINT,exit);
	zmq_pollitem_t polls[] = {
		{clients, 0, ZMQ_POLLIN, 0},
		{workers, 0, ZMQ_POLLIN, 0}
	};
	int length = sizeof(polls) / sizeof(zmq_pollitem_t);
	int i;
	int rs;
	zmsg_t* msg;
	pthread_t thread;
	for(i = 0; i < 5; i++) {
		pthread_create(&thread, NULL, worker, context);
		pthread_detach(thread);
	}
	for(;;) {
		rs = zmq_poll(polls, length, -1);
		if(rs < 0)
			puts("error!");
		if(rs = 0)
			continue;
		if(polls[0].revents & ZMQ_POLLIN) {
			#ifdef DEBUG
			puts("now: zstr_recv | main client");
			#endif 
			msg = zmsg_recv(clients);
			zmsg_send(&msg,workers);
			zmsg_destroy(&msg);
		}
		if(polls[1].revents & ZMQ_POLLIN) {
			#ifdef DEBUG
			puts("now: zstr_recv | main worker");
			#endif 
			msg = zmsg_recv(workers);
			zmsg_send(&msg,clients);
			zmsg_destroy(&msg);
		}
	}
	
	//zclock_sleep(10);
	zctx_destroy(&context);
	return 0; 
}
