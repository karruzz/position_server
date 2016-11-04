/*
 * Processer.h
 *
 *  Created on: Nov 4, 2016
 *      Author: kar
 */

#ifndef PROCESS_PROCESSER_H_
#define PROCESS_PROCESSER_H_

#include <stdlib.h>
#include <stdio.h>

#include "../Mathematics/quaternion.h"

struct GyroData
{
	ushort Id;
	ulong TimeStamp;
	struct Point Point;
};

void ConstructProcesser();
void ProcessGyro(const struct GyroData *data);
void DestructProcesser();

#endif /* PROCESS_PROCESSER_H_ */
