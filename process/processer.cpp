/*
 * processer.cpp
 *
 *  Created on: Nov 8, 2018
 *  Copyright © 2018-2080 Ilja Karasev. All rights reserved.
 *     License: GNU GPL 3
 */

#include "processer.h"
#include "cstdio"

bool Processer::calibration(const GyroData& data)
{
	static int i;
	if (i < 1000)
	{
		veloc_calibrated.x += data.veloc.x / 1000.0;
		veloc_calibrated.y += data.veloc.y / 1000.0;
		veloc_calibrated.z += data.veloc.z / 1000.0;
		i++;
		return true;
	}
	return false;
}

bool Processer::process(const BinsFrame &frame)
{
	static bool acs_ready = false, gyro_ready = false;

	if (frame.id == BINS_TYPE::GYRO)
		gyro_ready = process_gyro(GyroData { frame.timestamp, frame.data });
	else if (frame.id == BINS_TYPE::ACS)
		acs_ready = process_acs(AcsData { frame.timestamp, frame.data });

	if (acs_ready && gyro_ready) {
		gyro_ready = acs_ready = false;
		return true;
	}

	return false;
}

bool Processer::process_gyro(const GyroData& data)
{
	static uint64_t previous_timestamp = 0;
	if (calibration(data))
	{
		previous_timestamp = data.timestamp;
		return false;
	}

	// Gyroscope Update:
	double dt = (data.timestamp - previous_timestamp) * 1.0e-9;
	Axis velocities = data.veloc - veloc_calibrated;
	// https://stackoverflow.com/questions/46908345/integrate-angular-velocity-as-quaternion-rotation
	// dq / dt = 0.5 * q * v
	// dq =  0.5 * q * v  *dt

	Quaternion dq = q_body * 0.5 * velocities * dt;
	q_body = q_body + dq + q_body*(1 - q_body.length());

	// such integration is not very accurate
	// so we need re-normalization of the body orientation quaternion
	q_body = q_body / q_body.length();

	// "the estimated body orientation can be improved by pro-jecting the measurements from the accelerometer"
	// see good article (QUATERNION-BASED COMPLEMENTARY FILTER):
	// https://www.researchgate.net/publication/303598655_Quaternion-Based_Complementary_Filter_for_Attitude_Determination_of_a_Smartphone
	static const double u_a = 0.01; // 0≤ µa ≤1
	// acceleration due to gravity in the global frame of reference
	Axis acsel_in_global_frame = (q_body * _last.acsel * q_body.inversed()).axis(); // implicit conversion from axis body_initial to quat
	Quaternion q_correct = Quaternion(e_up, acsel_in_global_frame, u_a);
	q_body = q_body * q_correct;

	orientation.timestamp = data.timestamp;
	orientation.veloc = data.veloc;
	orientation.angle.x = q_body.roll();
	orientation.angle.y = q_body.pitch();
	orientation.angle.z = q_body.yaw();
	orientation.acsel = _last.acsel;

	previous_timestamp = data.timestamp;

	return true;
}

bool Processer::process_acs(const AcsData& data)
{
	_last = data;
	return true;
}

const Orientation& Processer::get_orientation()
{
	return orientation;
}

