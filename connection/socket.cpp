/*
 * socket.cpp
 *
 *  Created on: Nov 8, 2018
 *  Copyright © 2018-2080 Ilja Karasev. All rights reserved.
 *     License: GNU GPL 3
 */

#include "log.h"
#include "socket.h"
#include "sync_queue.h"

#include <sstream>
#include <atomic>

static std::atomic_bool stop(false);

void Socket::open_connection(const std::string& ip, int port)
{
	struct sockaddr_in server;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		throw std::runtime_error("could not create socket");

	server.sin_addr.s_addr = inet_addr(ip.c_str());
	server.sin_family = AF_INET;
	server.sin_port = htons(port);

	LogInfo() << "trying to connect to server, ip: " << ip << ", port: " << port << "...\n";

	if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
		throw std::runtime_error("could not connect");

	LogInfo() << "connected to server\n";
}

void Socket::close_connection()
{
	if (sock != -1)
		close(sock);
}

void Socket::start_listen(const std::string& ip, int port, LockQueue& queue)
{
	open_connection(ip, port);

	char server_reply[1000];
	int bytesReceived = 0, previousBytesReceived = 0;
	stop.store(false);
	while(!stop.load()) {
		bytesReceived = recv( sock, server_reply + previousBytesReceived, 800, MSG_DONTWAIT );
		if(bytesReceived < 0) {
			if (errno == EINTR) continue;
			if (errno == EAGAIN) {
				usleep(10);
				continue;
			}
			throw new std::runtime_error("recv failed");
		}

		previousBytesReceived = split(queue, (uint8_t*)server_reply, bytesReceived + previousBytesReceived);
	}

	close_connection();
}

void Socket::stop_listen()
{
	stop.store(true);
}

int Socket::split(LockQueue& queue, uint8_t* buff, int count)
{
	static const int MIN_LENGTH = 8;
	static const int MAX_LENGTH = 40;

	int crc_position, crc_value, packet_length, bytes_left = 0;
	for (int i = 0; i < count; i++) {
		// looking for a beginning of a packet in a tcp stream
		packet_length = buff[i];
		if (packet_length < MIN_LENGTH || packet_length > MAX_LENGTH) continue;

		// not enough bytes to parse a packet
		crc_position = i + packet_length;
		if (crc_position > count) {
			bytes_left = count - i;
			memmove(buff, buff + i, bytes_left);
			return bytes_left;
		}

		crc_value = packet_length;
		for (int j = 1; j < packet_length - 1; j++)
			crc_value ^= buff[i + j];

		if (crc_value == buff[i + packet_length - 1]) {
			size_t len = packet_length - 2;
			std::vector<uint8_t> data(len);
			memcpy(data.data(), buff + i + 1, len);
			queue.push(Packet { std::move(data) });
			i += packet_length - 1; // -1 because ++ in the for
		}
	}

	return 0; // todo: return right value
}
