/*
 * socket.h
 *
 *  Created on: Feb 12, 2017
 *  Copyright © 2017-2080 Ilja Karasev. All rights reserved.
 *     License: GNU GPL 3
 */

#ifndef CONNECTION_SOCKET_H_
#define CONNECTION_SOCKET_H_

#include <iostream>
#include <cstring>
#include <sys/socket.h>    //socket
#include <arpa/inet.h> //inet_addr
#include <unistd.h>
#include <errno.h>
#include <exception>

#include "sync_queue.h"

class Socket
{
	int sock;
	void open_connection(const std::string& ip, int port);
	void close_connection();

public:
	Socket() : sock(-1) {}
	~Socket() { close_connection(); }

	void start_listen(const std::string& ip, const int port, LockQueue& queue);
	void stop_listen();
};

#endif /* CONNECTION_SOCKET_H_ */
