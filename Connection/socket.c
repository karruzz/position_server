/*
 * socket.c
 *
 *  Created on: Oct 31, 2016
 *      Author: kar
 */

#include "socket.h"

static int Parse(struct LinkedQueue *queue, char* buffer, int count);

int Connect(const char* ip, int port, struct LinkedQueue *queue)
{
    int sock;
    struct sockaddr_in server;
    char server_reply[2000];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        printf("Could not create socket");
        return 1;
    }
    puts("Socket created");

    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
    puts("Connected\n");

    //keep communicating with server
    int bytesReceived;
    int bytesLeft = 0;
    while(1)
    {
        //Receive a reply from the server
    	bytesReceived = recv( sock, server_reply + bytesLeft, 2000, 0 );
        if(bytesReceived < 0)
        {
            puts("recv failed");
            break;
        }

        bytesLeft = Parse(queue, server_reply, bytesReceived);
    }

    close(sock);
	return 0;
}

static const int MinLength = 8;
static const int MaxLength = 40;
static int Parse(struct LinkedQueue *queue, char* buffer, int count)
{
	int crcPosition, crcValue, packetLength, bytesLeft, i;
	for (i = 0; i < count; i++)
	{
		packetLength = buffer[i];
		if (packetLength < MinLength || packetLength > MaxLength) continue;

		// осталось недостаточно байт
		crcPosition = i + packetLength;
		if (crcPosition > count) break;

		// подсчет контрольной суммы
		crcValue = packetLength;
		for (int j = 1; j < packetLength - 1; j++)
			crcValue ^= buffer[i + j];

		// все ок - копируем в очередь
		if (crcValue == buffer[i + packetLength - 1])
		{
	        DequeueLinkedMessage(queue, buffer + i + 1, packetLength - 2);
	        i += packetLength;
		}
	}

	bytesLeft = count - i - 1;
	// оставшиеся байты в начало массива
	if (bytesLeft > 0) memmove(buffer, buffer + i, bytesLeft);

	return bytesLeft;
}
