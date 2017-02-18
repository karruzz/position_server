/*
 * frame.cpp
 *
 *  Created on: Feb 03, 2017
 *  Copyright © 2017-2080 Ilja Karasev. All rights reserved.
 *     License: GNU GPL 3
 */

#include "frame.h"
#include <cstdio>

#include <arpa/inet.h> //inet_addr


static uint64_t be64toh_custom(uint64_t value){
	union Data { char val[8]; uint64_t result; } data;

	data.val[7] = value & 0xFF;
	data.val[6] = (value >> 8) & 0xFF;
	data.val[5] = (value >> 16) & 0xFF;
	data.val[4] = (value >> 24) & 0xFF;
	data.val[3] = (value >> 32) & 0xFF;
	data.val[2] = (value >> 40) & 0xFF;
	data.val[1] = (value >> 48) & 0xFF;
	data.val[0] = (value >> 56) & 0xFF;
	return data.result;
}

BinsFrame::BinsFrame()
{
}

BinsFrame::~BinsFrame()
{
}

bool BinsFrame::from_bytes(const uint8_t* buffer, size_t sz)
{
//	static double rotate_z = 0;
	id = htons(*(uint16_t *)buffer);
	timestamp = be64toh_custom(*((uint64_t *)(buffer + 2)));
	uint64_t tmpx = be64toh_custom(*((uint64_t *)(buffer + 10)));
	uint64_t tmpy = be64toh_custom(*((uint64_t *)(buffer + 18)));
	uint64_t tmpz = be64toh_custom(*((uint64_t *)(buffer + 26)));
	data.x    = *((double *)(&tmpx));
	data.y    = *((double *)(&tmpy));
	data.z    = *((double *)(&tmpz));

//	if (id == BINS_TYPE::ACS) {
//		data.x = 0;
//		data.y = 0;
//		data.z = 9.8;
//	} else {
//		data.x = 0;
//		data.y = 0;
//		data.z = 0;
//	}
	return true;
}

std::vector<uint8_t> BinsFrame::to_bytes()
{
	return std::vector<uint8_t>();
}

