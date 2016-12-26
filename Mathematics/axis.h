/*
 * axis.h
 *
 *  Created on: Dec 26, 2016
 *      Author: kar
 */

#ifndef MATHEMATICS_AXIS_H_
#define MATHEMATICS_AXIS_H_

#include <stdio.h>
#include <math.h>

struct Axis
{
	double X, Y, Z;
};

struct Axis AxisMultiply(const struct Axis *p, double k);
struct Axis AxisMinus(const struct Axis *p1, const struct Axis *p2);
struct Axis AxisCross(const struct Axis *p1, const struct Axis *p2);
double AxisLength(const struct Axis *p);

#endif /* MATHEMATICS_AXIS_H_ */
