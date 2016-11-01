/*
 * socket.h
 *
 *  Created on: Oct 31, 2016
 *      Author: kar
 */

#ifndef CONNECTION_SOCKET_H_
#define CONNECTION_SOCKET_H_

#include<stdio.h> //printf
#include<string.h>    //strlen
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
#include<unistd.h>
#include "syncQueue.h"

// returns socket
int Connect(const char* ip, const int port, struct LinkedQueue* queue);

#endif /* CONNECTION_SOCKET_H_ */
