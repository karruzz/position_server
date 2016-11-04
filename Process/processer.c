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
void ProcessGyro(const struct GyroData *data)
{
	if (Ustirate(data)) return;

	if (lastTime != 0)
	{
		double dt = 0.01;// (data->TimeStamp - lastTime) * 1.0e-9;
		struct Point omega = PointMinus(&(data->Point), &omega0);
		struct Point dangle = PointMultiply(&omega, dt);
		struct Quaternion qderive = QuatDerivative(&q, &dangle);
		q = QuatSum(&q, &qderive);

		fprintf(stdout, "roll: %3.3f; pitch: %3.3f; yaw %3.3f\n", QuatRoll(&q), QuatPitch(&q), QuatYaw(&q));
	}

	lastTime = data->TimeStamp;
}

void DestructProcesser()
{

}
