/*
 * quaternion.cpp
 *
 *  Created on: Feb 12, 2017
 *     Author: karruzz
 */

#include "quaternion.h"

void Quaternion::unit(double theta, double x_, double y_, double z_)
{
	s = cos(theta / 2.0);

	Axis a = { .x = x_, .y = y_, .z = z_ };
	a.normalize();

	double sn = sin(theta / 2.0);
	x = sn * a.x, y = sn * a.y, z = sn * a.z;
}

void Quaternion::normalize()
{
	*this = *this * (1.0 / Quaternion::length());
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

Quaternion Quaternion::conjugated()
{
	return {s, -x, -y, -z} ;
}

Quaternion Quaternion::inversed()
{
	return conjugated() * (1.0 / length());
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
