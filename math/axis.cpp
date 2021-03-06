/*
 * point.cpp
 *
 *  Created on: Nov 8, 2018
 *  Copyright © 2018-2080 Ilja Karasev. All rights reserved.
 *     License: GNU GPL 3
 */

#include "axis.h"
#include <cmath>

Axis operator* (const Axis& a, double k) {
	return { a.x * k, a.y * k, a.z * k};
}

Axis operator- (const Axis& a1, const Axis& a2) {
	return { a1.x - a2.x,
			 a1.y - a2.y,
			 a1.z - a2.z };
}

Axis Axis::cross(const Axis& a1, const Axis& a2)
{
	return { a1.y * a2.z - a1.z * a2.y,
			 a1.z * a2.x - a1.x * a2.z,
			 a1.x * a2.y - a1.y * a2.x };
}

double Axis::dot(const Axis& a1, const Axis& a2)
{
	return a1.x * a2.x + a1.y * a2.y + a1.z * a2.z;
}

double Axis::length() const
{
	return sqrt(x*x + y*y + z*z);
}
