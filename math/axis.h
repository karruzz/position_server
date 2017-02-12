/*
 * axis.h
 *
 *  Created on: Feb 12, 2017
 *     Author: karruzz
 */

#ifndef MATHEMATICS_AXIS_H_
#define MATHEMATICS_AXIS_H_

struct Axis
{
	double x, y, z;
	double length() const;
    void normalize();

	static Axis cross(const Axis& a1, const Axis& a2);
	static double dot(const Axis& a1, const Axis& a2);
};

Axis operator* (const Axis& a, double k);
Axis operator*(const Axis& a1, const Axis& a2);
Axis operator- (const Axis& a1, const Axis& a2);

#endif /* MATHEMATICS_AXIS_H_ */
