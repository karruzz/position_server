/*
 * frame.h
 *
 *  Created on: Nov 8, 2018
 *  Copyright © 2018-2080 Ilja Karasev. All rights reserved.
 *     License: GNU GPL 3
 */

#ifndef FRAME_H_
#define FRAME_H_

#include <vector>
#include <cstdint>

#include "axis.h"

enum BINS_TYPE{
	UNKNOWN,
	ACS = 0x1AA,
	GYRO = 0x1A9
};

struct Frame
{
	uint64_t timestamp;

	virtual bool from_bytes(const uint8_t* data, size_t sz) = 0;
	virtual std::vector<uint8_t> to_bytes() = 0;
};

struct BinsFrame : public Frame
{
	uint16_t id;
	Axis data;

	BinsFrame();
	~BinsFrame();

	virtual bool from_bytes(const uint8_t* buffer, size_t sz);
	virtual std::vector<uint8_t> to_bytes();
};

#endif /* FRAME_H_ */
