/*
 * socket.c
 *
 *  Created on: Oct 31, 2016
 *      Author: kar
 */

#include "socket.h"

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
    while(1)
    {
        //Receive a reply from the server
    	bytesReceived = recv(sock,  server_reply, 2000, 0);
        if(bytesReceived < 0)
        {
            puts("recv failed");
            break;
        }

        DequeueLinkedMessage(queue, server_reply, bytesReceived);
    }

    close(sock);
	return 0;
}
