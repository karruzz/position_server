/*
 * axis.c
 *
 *  Created on: Dec 26, 2016
 *      Author: kar
 */

#include "axis.h"

struct Axis AxisMultiply(const struct Axis *p, double k)
{
	struct Axis result;
	result.X = p->X * k; result.Y = p->Y * k; result.Z = p->Z * k;
	return result;
}

struct Axis AxisMinus(const struct Axis *p1, const struct Axis *p2)
{
	struct Axis result;
	result.X = p1->X - p2->X; result.Y = p1->Y - p2->Y; result.Z = p1->Z - p2->Z;
	return result;
}

struct Axis AxisCross(const struct Axis *p1, const struct Axis *p2)
{
	struct Axis result;
	result.X = p1->Y * p2->Z - p1->Z * p2->Y;
	result.Y = p1->Z * p2->X - p1->X * p2->Z;
	result.Z = p1->X * p2->Y - p1->Y * p2->X;

	return result;
}

double AxisLength(const struct Axis *p)
{
	return sqrt(p->X * p->X + p->Y * p->Y + p->Z * p->Z);
}
