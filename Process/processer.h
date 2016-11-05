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

struct GyroResult
{
	ulong TimeStamp;
	struct Point Angle;
	struct Point Omega;
	struct Point Acs;
};

void ConstructProcesser();
int ProcessGyro(const struct GyroData *data, struct GyroResult *result);
void DestructProcesser();

#endif /* PROCESS_PROCESSER_H_ */
