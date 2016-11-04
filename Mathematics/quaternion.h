/*
 * quaternion.h
 *
 *  Created on: Nov 4, 2016
 *      Author: kar
 */

#ifndef MATHEMATICS_QUATERNION_H_
#define MATHEMATICS_QUATERNION_H_

#include <stdio.h>
#include<math.h>

struct Quaternion
{
	double X, Y, Z, A;
};

struct Point
{
	double X, Y, Z;
};

struct Point PointMultiply(const struct Point *p, double k);
struct Point PointMinus(const struct Point *p1, const struct Point *p2);

struct Quaternion QuatFromPoint(const struct Point *p);
struct Quaternion QuatUnit(double angle, double x, double y, double z);
struct Quaternion QuatMinus(const struct Quaternion *q1);

struct Quaternion QuatSum(const struct Quaternion *q1, const struct Quaternion *q2);
struct Quaternion QuatSum1(const struct Quaternion *q, double s);

struct Quaternion QuatMultiply(const struct Quaternion *q1, const struct Quaternion *q2);
struct Quaternion QuatMultiply1(const struct Quaternion *q, double k);

struct Quaternion QuatDerivative(const struct Quaternion *q, const struct Point *omega);

double QuatNorm(const struct Quaternion *q);

double QuatRoll(const struct Quaternion *q);  // крен
double QuatPitch(const struct Quaternion *q); // тангаж
double QuatYaw(const struct Quaternion *q);   // рысканье

#endif /* MATHEMATICS_QUATERNION_H_ */
