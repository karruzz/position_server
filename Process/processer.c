/*
 * processer.c
 *
 *  Created on: Nov 4, 2016
 *      Author: kar
 */

#include "processer.h"

#define ACS_ID 0x1AA
#define GYRO_ID 0x1A9

static struct Quaternion _q;
static struct AcsData _a;
static struct Quaternion _qg;

void ConstructProcesser()
{
	_q = QuatVersor(0, 0, 0, 1);

	struct Point g = {.X = 0, .Y = 0, .Z = 1.0};
	_qg = QuatFromPoint( &g );
}

static struct Point omega0;
static int i;
static int Ustirate(const struct GyroData *data)
{
	if (i < 1000)
	{
		omega0.X += data->Omega.X / 1000.0;
		omega0.Y += data->Omega.Y / 1000.0;
		omega0.Z += data->Omega.Z / 1000.0;
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
	struct Point omega = PointMinus(&(data->Omega), &omega0);
	struct Point dangle = PointMultiply(&omega, dt);
	struct Quaternion qderive = QuatDerivative(&_q, &dangle);
	_q = QuatSum(&_q, &qderive);

	result->TimeStamp = data->TimeStamp;
	result->Omega = data->Omega;
	result->Angle.X = QuatRoll(&_q);
	result->Angle.Y = QuatPitch(&_q);
	result->Angle.Z = QuatYaw(&_q);
	result->Acs = _a.Acs;

	lastTime = data->TimeStamp;

	return 0;
}

int ProcessAcs(const struct AcsData *data)
{
	_a = *data;
	return 0;
}

void DestructProcesser()
{

}
