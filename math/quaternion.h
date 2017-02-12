/*
 * quaternion.h
 *
 *  Created on: Feb 12, 2017
 *     Author: karruzz
 */

#ifndef MATHEMATICS_QUATERNION_H_
#define MATHEMATICS_QUATERNION_H_

#include <axis.h>

#include <cmath>

struct Quaternion
{
	double s;
	double x, y, z;

	Quaternion() {}
	Quaternion(double s_, double x_, double y_, double z_) { s = s_; x = x_; y = y_; z = z_; }
	Quaternion(const Axis& p) { s = 0; x = p.x; y = p.y; z = p.z; }

	Axis axis() { return { x, y, z }; }

	void unit(double angle, double x, double y, double z);
	void normalize();

	Quaternion inversed();
	Quaternion conjugated();

	double length() const;

	double roll() const;
	double pitch() const;
	double yaw() const;
};

Quaternion operator+ (const Quaternion& q1, const Quaternion& q2);
Quaternion operator+ (const Quaternion& q, double s);
Quaternion operator* (const Quaternion& q1, const Quaternion& q2);
Quaternion operator* (const Quaternion& q, double s);

#endif /* MATHEMATICS_QUATERNION_H_ */
