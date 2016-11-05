/*
 * processer.c
 *
 *  Created on: Nov 4, 2016
 *      Author: kar
 */

#include "processer.h"

#define ACS_ID 0x1AA
#define GYRO_ID 0x1A9

static struct Quaternion q;

void ConstructProcesser()
{
	q = QuatUnit(0, 0, 0, 1);
}

static struct Point omega0;
static int i;
static int Ustirate(const struct GyroData *data)
{
	if (i < 1000)
	{
		omega0.X += data->Point.X / 1000.0;
		omega0.Y += data->Point.Y / 1000.0;
		omega0.Z += data->Point.Z / 1000.0;
		i++;
		return 1;
	}
	return 0;
}

static ulong lastTime = 0;
int ProcessGyro(const struct GyroData *data, struct GyroResult *result)
{
	if (Ustirate(data))
	{
		lastTime = data->TimeStamp;
		return 1;
	}

	double dt = (data->TimeStamp - lastTime) * 1.0e-9;
	struct Point omega = PointMinus(&(data->Point), &omega0);
	struct Point dangle = PointMultiply(&omega, dt);
	struct Quaternion qderive = QuatDerivative(&q, &dangle);
	q = QuatSum(&q, &qderive);

	result->TimeStamp = data->TimeStamp;
	result->Omega = data->Point;
	result->Angle.X = QuatRoll(&q);
	result->Angle.Y = QuatPitch(&q);
	result->Angle.Z = QuatYaw(&q);

	lastTime = data->TimeStamp;

	return 0;
}

void DestructProcesser()
{

}
