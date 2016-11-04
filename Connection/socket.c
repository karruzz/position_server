/*
 * socket.c
 *
 *  Created on: Oct 31, 2016
 *      Author: kar
 */

#include "socket.h"

static volatile int stop;

static int Parse(struct LinkedQueue *queue, char* buffer, int count);

int StartListen(const char* ip, int port, struct LinkedQueue *queue)
{
    int sock;
    struct sockaddr_in server;
    char server_reply[1000];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        printf("Could not create socket");
        return 1;
    }

    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
    fprintf(stdout, "Connected to server, ip: %s, port: %d\n", ip, port);

    //keep communicating with server
    int bytesReceived;
    int bytesLeft = 0;

    stop = 0;
    while(!stop)
    {
        //Receive a reply from the server
    	bytesReceived = recv( sock, server_reply + bytesLeft, 800, MSG_DONTWAIT );
        if(bytesReceived < 0)
        {
        	if (errno == EINTR) continue;
			if (errno == EAGAIN)
			{
				usleep(10);
				continue;
			}

			perror("recv failed, socket closed");
		    close(sock);
            return 1;
        }

        bytesLeft = Parse(queue, server_reply, bytesReceived + bytesLeft);
    }

    close(sock);
	return 0;
}

int StopListen()
{
	stop = 1;
	return 0;
}

static const int MinLength = 8;
static const int MaxLength = 40;
static int Parse(struct LinkedQueue *queue, char* buffer, int count)
{
	int crcPosition, crcValue, packetLength, bytesLeft;
	for (int i = 0; i < count; i++)
	{
		packetLength = (unsigned char)buffer[i];
		if (packetLength < MinLength || packetLength > MaxLength) continue;

		// осталось недостаточно байт для парсинга пакета
		crcPosition = i + packetLength;
		if (crcPosition > count) {
			bytesLeft = count - i;
			memmove(buffer, buffer + i, bytesLeft);
			return bytesLeft;
		}

		// подсчет контрольной суммы
		crcValue = packetLength;
		for (int j = 1; j < packetLength - 1; j++)
			crcValue ^= (unsigned char)buffer[i + j];

		// все ок - копируем в очередь
		if (crcValue == (unsigned char)buffer[i + packetLength - 1])
		{
	        DequeueLinkedMessage(queue, buffer + i + 1, packetLength - 2);
	        i += packetLength - 1; // -1 потому что будет еще ++ в while
		}
	}

	return 0;
}
