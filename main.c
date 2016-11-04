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

#define ACS_ID 0x1AA
#define GYRO_ID 0x1A9

static void *listen_messages_function( void *arg )
{
    puts("Start process_messages_function");

	StartListen("192.168.0.101", 12346, (struct LinkedQueue *) arg);
	return 0;
}


static void *print_messages_function( void *queue )
{
    puts("Start print_messages_function");

    ulong acsCount = 0, gyroCount = 0;
	char server_reply[50];
    while(1)
    {
    	EnqueueLinkedMessage((struct LinkedQueue *)queue, server_reply);

    	ushort id = htons(*(ushort *)server_reply);
    	ulong time = be64toh(*((ulong *)(server_reply + 2)));
    	if (id == ACS_ID)
    	{
    		if (time - acsCount != 1) fprintf(stdout, "error acs step\n");
    		acsCount = time;
    	}

    	if (id == GYRO_ID)
    	{
    		if (time - gyroCount != 1) fprintf(stdout, "error gyro step\n");
    		gyroCount = time;
    	}

		if (id != ACS_ID && id != GYRO_ID) fprintf(stdout, "strange id: %hX\n", id);

    	//fprintf(stdout, "< %hX  %lu\n", id , time );
    }
    return 0;
}

int main()
{
	struct LinkedQueue queue = ConstructLinkedQueue(8192);

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

