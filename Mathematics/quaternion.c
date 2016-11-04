/*
 * quaternion.c
 *
 *  Created on: Nov 4, 2016
 *      Author: kar
 */

#include "quaternion.h"

struct Point PointMultiply(const struct Point *p, double k)
{
	struct Point result;
	result.X = p->X * k; result.Y = p->Y * k; result.Z = p->Z * k;
	return result;
}

struct Point PointMinus(const struct Point *p1, const struct Point *p2)
{
	struct Point result;
	result.X = p1->X - p2->X; result.Y = p1->Y - p2->Y; result.Z = p1->Z - p2->Z;
	return result;
}

struct Quaternion QuatFromPoint(const struct Point *p)
{
	struct Quaternion q;
	q.A = 0; q.X = p->X; q.Y = p->Y; q.Z = p->Z;
	return q;
}

struct Quaternion QuatUnit(double angle, double x, double y, double z)
{
	double norm = 1 / sqrt(x*x + y*y + z*z);

	x *= norm; y *= norm; z *= norm;

	double c = cos(angle / 2.0);
	double s = sin(angle / 2.0);

	struct Quaternion q;

	q.A = c; q.X = s * x; q.Y = s * y; q.Z = s * z;

	return q;
}

struct Quaternion QuatSum(const struct Quaternion *q1, const struct Quaternion *q2)
{
	struct Quaternion q;
	q.A = q1->A + q2->A; q.X = q1->X + q2->X; q.Y = q1->Y + q2->Y; q.Z = q1->Z + q2->Z;
	return q;
}

struct Quaternion QuatSum1(const struct Quaternion *q1, double s)
{
	struct Quaternion q;
	q.A = q1->A + s; q.X = q1->X; q.Y = q1->Y; q.Z = q1->Z;
	return q;
}

struct Quaternion QuatMultiply(const struct Quaternion *q1, const struct Quaternion *q2)
{
	struct Quaternion q;
	q.A = q1->A * q2->A - q1->X * q2->X - q1->Y * q2->Y - q1->Z * q2->Z;
	q.X = q1->A * q2->X - q1->X * q2->A - q1->Y * q2->Z - q1->Z * q2->Y;
	q.Y = q1->A * q2->Y - q1->X * q2->Z - q1->Y * q2->A - q1->Z * q2->X;
	q.Z = q1->A * q2->Z - q1->X * q2->Y - q1->Y * q2->X - q1->Z * q2->A;
	return q;
}

struct Quaternion QuatMultiply1(const struct Quaternion *q1, double s)
{
	struct Quaternion q;
	q.A = q1->A * s; q.X = q1->X * s; q.Y = q1->Y * s; q.Z = q1->Z * s;
	return q;
}

struct Quaternion QuatMinus(const struct Quaternion *q1)
{
	struct Quaternion q;
	q.A = -q1->A; q.X = -q1->X; q.Y = -q1->Y; q.Z = -q1->Z;
	return q;
}

struct Quaternion QuatDerivative(const struct Quaternion *q, const struct Point *omega)
{
	struct Quaternion om = QuatFromPoint(omega);
	struct Quaternion q1 = QuatMultiply1(q, 0.5);
	return QuatMultiply(&q1, &om);
}

double QuatNorm(const struct Quaternion *q)
{
	return sqrt(q->A * q->A + q->X * q->X + q->Y * q->Y + q->Z * q->Z);
}

double QuatRoll(const struct Quaternion *q)
{
	return atan2(2.0*(q->Y * q->Z + q->A * q->X),
			    (q->A * q->A + q->Z * q->Z - q->Y * q->Y - q->X * q->X ));
}

double QuatPitch(const struct Quaternion *q)
{
	return asin( -2.0*(q->X * q->Z - q->A * q->Y) );
}

double QuatYaw(const struct Quaternion *q)
{
	return atan2(2.0*(q->X * q->Y + q->A * q->Z),
			    (q->A * q->A - q->Z * q->Z - q->Y * q->Y + q->X * q->X ));
}
