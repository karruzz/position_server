/*
 * processer.cpp
 *
 *  Created on: Feb 12, 2017
 *     Author: karruzz
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
	q_body = q_body + dq;

	// such integration is not very accurate
	// so we need re-normalization of the body orientation quaternion
	q_body.normalize();

	// "the estimated body orientation can be improved by pro-jecting the measurements from the accelerometer"
	// see good article (QUATERNION-BASED COMPLEMENTARY FILTER):
	// https://www.researchgate.net/publication/303598655_Quaternion-Based_Complementary_Filter_for_Attitude_Determination_of_a_Smartphone
	static const Axis e_up = Axis { 0, 0, 1 };
	// acceleration due to gravity in the global frame of reference (GFR)
	Axis acsel_gfr = (q_body * _last.acsel * q_body.inversed()).axis(); // implicit conversion from axis body_initial to quat

	Axis n_up = Axis::cross(acsel_gfr, e_up);
	double gamma_a = acos(Axis::dot(acsel_gfr, e_up) / acsel_gfr.length());

	static const double u_a = 0.01; // 0≤ µa ≤1
	Quaternion q_correct;
	q_correct.unit(u_a * gamma_a, n_up.x, n_up.y, n_up.z);
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

