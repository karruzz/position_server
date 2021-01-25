/*
 * axis.h
 *
 *  Created on: Nov 8, 2018
 *  Copyright © 2018-2080 Ilja Karasev. All rights reserved.
 *     License: GNU GPL 3
 */

#ifndef MATHEMATICS_POINT_H_
#define MATHEMATICS_POINT_H_

struct Axis
{
	double x, y, z;
	double length() const;

	static Axis cross(const Axis& a1, const Axis& a2);
	static double dot(const Axis& a1, const Axis& a2);
};

Axis operator* (const Axis& a, double k);
Axis operator*(const Axis& a1, const Axis& a2);
Axis operator- (const Axis& a1, const Axis& a2);

#endif /* MATHEMATICS_POINT_H_ */
