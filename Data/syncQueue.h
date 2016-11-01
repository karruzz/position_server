/*
 * queue.h
 *
 *  Created on: Oct 31, 2016
 *      Author: kar
 */

#ifndef DATA_SYNCQUEUE_H_
#define DATA_SYNCQUEUE_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

struct LinkedMessage
{
	int Length;
	char* Data;
	struct LinkedMessage* Next;
	int PositionInBuffer;
};

struct LinkedQueue
{
	struct LinkedMessage* Head;
	struct LinkedMessage* Tail;
	int Count;

	char *Buffer;
	int BufferSize; // в байтах
	int BufferHead;
	int BufferTail;

	pthread_mutex_t Sync;
	pthread_cond_t DataAvailable;
};

struct LinkedQueue ConstructLinkedQueue(int size);
int DequeueLinkedMessage(struct LinkedQueue* queue, const char* data, int length);
int EnqueueLinkedMessage(struct LinkedQueue* queue, char* data);
void DestructLinkedQueue(const struct LinkedQueue* queue);

#endif /* DATA_SYNCQUEUE_H_ */
