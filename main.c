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


static void *listen_messages_function( void *arg )
{
    puts("Start process_messages_function");

	StartListen("192.168.0.101", 12346, (struct LinkedQueue *) arg);
	return 0;
}

static void *print_messages_function( void *queue )
{
    puts("Start print_messages_function");

	char server_reply[50];
    while(1)
    {
    	EnqueueLinkedMessage((struct LinkedQueue *)queue, server_reply);

    	ushort id = htons(*(ushort *)server_reply);
    	ulong time = be64toh(*((ulong *)(server_reply + 2)));

    	fprintf(stdout, "< %hX  %lu\n", id , time );
    }
    return 0;
}

int main()
{
	struct LinkedQueue queue = ConstructLinkedQueue(1024);

    pthread_t sniffer_thread, print_thread;
    if (pthread_create( &sniffer_thread , NULL , &listen_messages_function , (void *) &queue) < 0)
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

