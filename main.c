/*
 * main.c
 *
 *  Created on: Oct 31, 2016
 *      Author: kar
 */


#include <stdio.h>
#include <pthread.h>

#include "socket.h"
#include "syncQueue.h"


static void *process_messages_function( void *arg )
{
    puts("Start process_messages_function");

	Connect("192.168.0.102", 12346, (struct LinkedQueue *) arg);
	return 0;
}

static void *print_messages_function( void *queue )
{
    puts("Start print_messages_function");

	char server_reply[500];
	int length;
    while(1)
    {
    	length = EnqueueLinkedMessage((struct LinkedQueue *)queue, server_reply);
    	server_reply[length] ='\0';
    	fprintf(stdout, "%s\n", server_reply);
    	//puts(server_reply);
    }
    return 0;
}

int main()
{
	struct LinkedQueue queue = ConstructLinkedQueue(1024);

    pthread_t sniffer_thread, print_thread;
    if (pthread_create( &sniffer_thread , NULL , &process_messages_function , (void *) &queue) < 0)
    {
        perror("could not create thread");
        exit(EXIT_FAILURE);
    }

    if (pthread_create( &print_thread , NULL , &print_messages_function , (void *) &queue) < 0)
    {
        perror("could not create thread");
        exit(EXIT_FAILURE);
    }

    pthread_join( sniffer_thread, NULL);
 	pthread_join( print_thread, NULL);

	DestructLinkedQueue(&queue);
	exit(EXIT_SUCCESS);
}

