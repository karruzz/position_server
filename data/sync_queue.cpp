/*
 * sync_queue.cpp
 *
 *  Created on: Nov 8, 2018
 *  Copyright © 2018-2080 Ilja Karasev. All rights reserved.
 *     License: GNU GPL 3
 */
#include "sync_queue.h"

void LockQueue::push(Packet&& frame)
{
	std::lock_guard<std::mutex> lock(mtx);

	static int warn_size = 0;
	if (size > MAX_SIZE) {
		if (++warn_size > 100) {
			LogWarning() << "data list overflow\n";
			warn_size = 0;
		}
		return;
	}
	++size;

	list.push_front(frame);
}

bool LockQueue::empty() {
	std::lock_guard<std::mutex> lock(mtx);
	return list.empty();
}

Packet LockQueue::pop()
{
	std::lock_guard<std::mutex> lock(mtx);
	--size;
	Packet f = list.back();
	list.pop_back();
	return f;
}


