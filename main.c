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
#include "processer.h"
#include "writer.h"

#define ACS_ID 0x1AA
#define GYRO_ID 0x1A9

static int fd;

static void *listen_messages_function( void *arg )
{
    puts("Start process_messages_function");

	StartListen("192.168.0.102", 12346, (struct LinkedQueue *) arg);
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
    	ulong xbits = be64toh(*((ulong *)(server_reply + 10)));
    	ulong ybits = be64toh(*((ulong *)(server_reply + 18)));
    	ulong zbits = be64toh(*((ulong *)(server_reply + 26)));

    	if (id != GYRO_ID) continue;

		struct GyroData gyro;
		gyro.Id = GYRO_ID;
		gyro.TimeStamp = time;

		gyro.Point.X = *((double *)(&xbits));
		gyro.Point.Y = *((double *)(&ybits));
		gyro.Point.Z = *((double *)(&zbits));

		struct GyroResult result;
		if (ProcessGyro(&gyro, &result)) continue;

		fprintf(stdout, "X %3.3f; Y %3.3f; Z %3.3f\n", result.Angle.X, result.Angle.Y, result.Angle.Z );

		WriteToFile(fd, &result);
    }
    return 0;
}

int main()
{
	fd = CreateWriter("out.dat");
	if (fd == -1) exit(EXIT_FAILURE);

	struct LinkedQueue queue = ConstructLinkedQueue(8192);
	ConstructProcesser();

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
	CloseWriter(fd);
	exit(EXIT_SUCCESS);
}
