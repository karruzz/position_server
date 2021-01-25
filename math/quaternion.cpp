/*
 * quaternion.cpp
 *
 *  Created on: Nov 8, 2018
 *  Copyright © 2018-2080 Ilja Karasev. All rights reserved.
 *     License: GNU GPL 3
 */

#include "quaternion.h"

void Quaternion::unit(double theta, double x_, double y_, double z_)
{
	s = cos(theta / 2.0);

	double K = 1 / sqrt(x_*x_ + y_*y_ + z_*z_);
	double sn = sin(theta / 2.0);

	x = K * sn * x_;
	y = K * sn * y_;
	z = K * sn * z_;
}

Quaternion operator+ (const Quaternion& q1, const Quaternion& q2)
{
	return { q1.s + q2.s, q1.x + q2.x, q1.y + q2.y, q1.z + q2.z };
}

Quaternion operator+ (const Quaternion& q, double s)
{
	return { q.s + s, q.x, q.y, q.z };
}

Quaternion operator* (const Quaternion& q1, const Quaternion& q2)
{
	Quaternion q;
	q.s = q1.s * q2.s - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
	q.x = q1.s * q2.x + q1.x * q2.s + q1.y * q2.z - q1.z * q2.y;
	q.y = q1.s * q2.y - q1.x * q2.z + q1.y * q2.s + q1.z * q2.x;
	q.z = q1.s * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.s;
	return q;
}

Quaternion operator* (const Quaternion& q, double s)
{
	return { q.s * s, q.x * s, q.y * s, q.z * s };
}

Quaternion operator/ (const Quaternion& q, double k)
{
	return { q.s / k, q.x / k, q.y / k, q.z / k };
}

Quaternion Quaternion::conjugated()
{
	return {s, -x, -y, -z} ;
}

Quaternion Quaternion::inversed()
{
	return conjugated() / length();
}

double Quaternion::length() const
{
	return sqrt(s*s + x*x + y*y + z*z);
}

double Quaternion::roll() const
{
	return atan2( 2.0*(y*z + s*x), (s*s + z*z - y*y - x*x) );
}

double Quaternion::pitch() const
{
	return asin( -2.0*(x*z - s*y) );
}

double Quaternion::yaw() const
{
	return atan2( 2.0*(x*y + s*z), (s*s - z*z - y*y + x*x ));
}
