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
	result.BufferHead = 0;
	result.BufferTail = 0;

	result.Head = NULL;
	result.Tail = NULL;
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
	//fprintf(stdout, "Dequeue, length: %d\n", length);

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


	//fprintf(stdout, "freeSpace: %d, count: %d, tail: %d, head: %d\n",freeSpace, queue->Count,queue->BufferTail,queue->BufferHead);

	if (freeSpace < messageLength)
	{
		fprintf(stdout, "Can't dequeue, messageLength: %d, tail: %d, head: %d\n", messageLength, queue->BufferTail, queue->BufferHead);
		pthread_mutex_unlock( &(queue->Sync) );
		return -1;
	}

	// копируем данные
	struct LinkedMessage *newMessage = (struct LinkedMessage *)(queue->Buffer + queue->BufferHead);
	newMessage->Length = length;
	newMessage->Data = memmove(queue->Buffer + queue->BufferHead + linkedMessageSize, data, length);
	newMessage->Next = NULL;
	newMessage->PositionInBuffer = queue->BufferHead;

//	fprintf(stdout, "queue->Buffer + queue->BufferHead: %d\n", queue->Buffer + queue->BufferHead);
//	fprintf(stdout, "newMessage: %d\n", newMessage);
//	fprintf(stdout, "newMessage->Length: %d\n", newMessage->Length);
//	fprintf(stdout, "&(newMessage->Length): %d\n", &(newMessage->Length));
//	fprintf(stdout, "linkedMessageSize: %d\n", linkedMessageSize);

	//exit(EXIT_FAILURE);

	//newMessage.PositionInBuffer =  queue->BufferHead;
  //  fprintf(stdout, "newMessage.PositionInBuffer: %d\n", newMessage.PositionInBuffer);

	queue->BufferHead += length + linkedMessageSize;

	//fprintf(stdout, "new message: %d\n", (int)(&newMessage));
	// вставляем newMessage в очередь
	if (queue->Tail == NULL)
	{
		queue->Tail = newMessage;
	//	puts("Add tail");
	}
	if (queue->Head != NULL)
	{
	//    fprintf(stdout, "add next, tail: %d, head: %d, next: %d\n", (int)(queue->Tail), (int)(queue->Head), (int)(queue->Head->Next));
		queue->Head->Next = newMessage;
	//    fprintf(stdout, "add next, tail: %d, head: %d, next: %d\n", (int)(queue->Tail), (int)(queue->Head), (int)(queue->Head->Next));
	}
	queue->Head = newMessage;
  //  fprintf(stdout, "add next, tail: %d, head: %d, next: %d\n", (int)(queue->Tail), (int)(queue->Head), (int)(queue->Head->Next));

	(queue->Count)++;
    //fprintf(stdout, "queue->BufferHead: %d\n", queue->BufferHead);
  //  fprintf(stdout, "queue->Count: %d\n", queue->Count);

	pthread_cond_signal( &(queue->DataAvailable) );

	pthread_mutex_unlock( &(queue->Sync) );

	return 0;
}

int EnqueueLinkedMessage(struct LinkedQueue *queue, char *data)
{
  //  puts("Enqueue");

	pthread_mutex_lock( &(queue->Sync) );

    if (queue->Count == 0) pthread_cond_wait( &(queue->DataAvailable), &(queue->Sync) );

    int length = queue->Tail->Length;
    data = queue->Tail->Data;
    (queue->Count)--;

    // очередь не пустая
	if(queue->Tail->Next != NULL) {
	 //   fprintf(stdout, "queue->Tail->PositionInBuffer: %d\n", queue->Tail->PositionInBuffer);
	 //   fprintf(stdout, "queue->Tail->Next->PositionInBuffer: %d\n", queue->Tail->Next->PositionInBuffer);
	    queue->Tail = queue->Tail->Next;
		queue->BufferTail = queue->Tail->PositionInBuffer;
	  //  fprintf(stdout, "queue->Tail->PositionInBuffer: %d\n", queue->Tail->PositionInBuffer);
	}
	// очередь пустая
	else
	{
		queue->Head = queue->Tail = NULL;
		queue->BufferTail = queue->BufferHead;
	}

  //  fprintf(stdout, "queue->BufferTail: %d\n", queue->BufferTail);
  //  fprintf(stdout, "queue->Count: %d\n", queue->Count);

	pthread_mutex_unlock( &(queue->Sync) );

	return length;
}

void DestructLinkedQueue(const struct LinkedQueue *queue)
{
	free(queue->Buffer);
}


