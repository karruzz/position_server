/*
 * queue.c
 *
 *  Created on: Oct 31, 2016
 *      Author: kar
 */

#include "syncQueue.h"

static const int linkedMessageSize = sizeof(struct LinkedMessage);

struct LinkedQueue ConstructLinkedQueue(int size)
{
	struct LinkedQueue result;
	result.Buffer = malloc(size);
	if (!result.Buffer) {
		perror("malloc. Error");
		exit(EXIT_FAILURE);
	}
	result.BufferSize = size;
	result.BufferHead = result.BufferTail = 0;

	result.Head = result.Tail = NULL;
	result.Count = 0;

	if (pthread_mutex_init( &(result.Sync), NULL ))
	{
		perror("pthread_mutex_init. Error");
		exit(EXIT_FAILURE);
	}

	if (pthread_cond_init( &(result.DataAvailable), NULL ))
	{
		perror("pthread_cond_init. Error");
		exit(EXIT_FAILURE);
	}

	return result;
}

int DequeueLinkedMessage(struct LinkedQueue *queue, const char *data, int length)
{
    pthread_mutex_lock( &(queue -> Sync) );

	// подсчет свободного места
    int freeSpace = 0;
    int messageLength = linkedMessageSize + length;

    freeSpace = queue->BufferHead >= queue->BufferTail
    		? queue->BufferSize - queue->BufferHead
    		: queue->BufferTail - queue->BufferHead;

    if (freeSpace < messageLength)
    {
    	if (queue->BufferTail == queue->BufferHead)
    	{
    		queue->BufferTail = queue->BufferHead = 0;
    		freeSpace = queue->BufferSize;
    	}
    	else if (queue->BufferHead > queue->BufferTail && queue->BufferTail > messageLength)
    	{
    		queue->BufferHead = 0;
    		freeSpace = queue->BufferTail;
    	}
    }

	if (freeSpace < messageLength)
	{
		fprintf(stdout, "Can't dequeue element, not enough space, messageLength: %d, tail: %d, head: %d\n", messageLength, queue->BufferTail, queue->BufferHead);
		pthread_mutex_unlock( &(queue->Sync) );
		return -1;
	}

	// копируем данные
	struct LinkedMessage *newMessage = (struct LinkedMessage *)(queue->Buffer + queue->BufferHead);
	newMessage->Length = length;
	newMessage->Data = memmove(queue->Buffer + queue->BufferHead + linkedMessageSize, data, length);
	newMessage->Next = NULL;
	newMessage->PositionInBuffer = queue->BufferHead;

	queue->BufferHead += length + linkedMessageSize;

	if (queue->Tail == NULL) queue->Tail = newMessage;
	if (queue->Head != NULL) queue->Head->Next = newMessage;
	queue->Head = newMessage;

	(queue->Count)++;

	pthread_cond_signal( &(queue->DataAvailable) );

	pthread_mutex_unlock( &(queue->Sync) );

	return 0;
}

int EnqueueLinkedMessage(struct LinkedQueue *queue, char *data)
{
	pthread_mutex_lock( &(queue->Sync) );

    if (queue->Count == 0) pthread_cond_wait( &(queue->DataAvailable), &(queue->Sync) );
    // вызван DestructLinkedQueue
    if (queue->Count == 0) return -1;

    int length = queue->Tail->Length;
    data = memmove(data, queue->Tail->Data, length);
    (queue->Count)--;

    // очередь не пустая
	if(queue->Tail->Next != NULL) {
	    queue->Tail = queue->Tail->Next;
		queue->BufferTail = queue->Tail->PositionInBuffer;
	}
	// очередь пустая
	else
	{
		queue->Head = queue->Tail = NULL;
		queue->BufferTail = queue->BufferHead;
	}

	pthread_mutex_unlock( &(queue->Sync) );

	return length;
}

void DestructLinkedQueue(const struct LinkedQueue *queue)
{
	//pthread_cond_broadcast( &(queue->DataAvailable) );
	free(queue->Buffer);
}


