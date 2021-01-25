/*
 * quaternion.h
 *
 *  Created on: Nov 8, 2018
 *  Copyright © 2018-2080 Ilja Karasev. All rights reserved.
 *     License: GNU GPL 3
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
	Quaternion(const Axis& n, const Axis& m, double portion) {
		Axis q = Axis::cross(n, m);
		double angle = asin( q.length() / (n.length() * m.length()) );
		unit(portion * angle, q.x, q.y, q.z);
	}

	Axis axis() { return { x, y, z }; }

	void unit(double theta, double x, double y, double z);

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
Quaternion operator/ (const Quaternion& q, double k);

#endif /* MATHEMATICS_QUATERNION_H_ */
