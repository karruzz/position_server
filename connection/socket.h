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

#include <errno.h>
#include <exception>
#include <cstdio>

#include "sync_queue.h"

class Socket
{
public:
	Socket() : sock(-1) {}
	~Socket() { close_connection(); }

	void start_listen(const std::string& ip, const int port, LockQueue& queue);
	void stop_listen();

private:
	int sock;
	void open_connection(const std::string& ip, int port);
	void close_connection();
	int split(LockQueue& queue, unsigned char* buffer, int count);
};

#endif /* CONNECTION_SOCKET_H_ */
