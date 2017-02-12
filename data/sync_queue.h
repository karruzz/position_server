/*
 * sync_queue.h
 *
 *  Created on: Jan 7, 2017
 *     Author: karruzz
 */

#ifndef SYNCQUEUE_H_
#define SYNCQUEUE_H_

#include "log.h"
#include "frame.h"

#include <mutex>
#include <memory>
#include <list>
#include <vector>

struct Packet {
	std::vector<uint8_t> bytes;
};

class LockQueue
{
	const int MAX_SIZE = 100;
	size_t size;

	std::mutex mtx;
	std::list<Packet> list;

public:
	LockQueue() : size(0) {}

	void push(Packet&& node);
	Packet pop();
	bool empty();
};

#endif /* SYNCQUEUE_H_ */

