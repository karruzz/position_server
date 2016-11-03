/*
 * socket.h
 *
 *  Created on: Oct 31, 2016
 *      Author: kar
 */

#ifndef CONNECTION_SOCKET_H_
#define CONNECTION_SOCKET_H_

#include <stdio.h> //printf
#include <string.h>    //strlen
#include <sys/socket.h>    //socket
#include <arpa/inet.h> //inet_addr
#include <unistd.h>
#include <errno.h>
#include <unistd.h>
#include "syncQueue.h"

// returns socket
int StartListen(const char* ip, const int port, struct LinkedQueue* queue);
int StopListen();

#endif /* CONNECTION_SOCKET_H_ */
