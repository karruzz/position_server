/*
 * processer.h
 *
 *  Created on: Feb 12, 2017
 *  Copyright © 2017-2080 Ilja Karasev. All rights reserved.
 *     License: GNU GPL 3
 */

#ifndef PROCESS_PROCESSER_H_
#define PROCESS_PROCESSER_H_

#include "frame.h"
#include "quaternion.h"

struct Orientation
{
//	const uint64_t header = 0xA5A5;
	uint64_t timestamp;
	Axis angle; // roll, pitch, yaw
	Axis veloc; // gyros
	Axis acsel;
};

class Processer
{
	struct GyroData {
		uint64_t timestamp;
		Axis veloc;
	};

	struct AcsData {
		uint64_t timestamp;
		Axis acsel;
	};

	Quaternion q_body; // by gyroscope
	AcsData _last; // last data from acsel (because gyro and acsel in different packets)

	Orientation orientation;

	Axis veloc_calibrated;

	bool calibration(const GyroData& data);
	bool process_gyro(const GyroData& data);
	bool process_acs (const AcsData& data);
public:
	Processer()
	{
		// TODO: collect in the begging, now just gravity
		veloc_calibrated = Axis { 0, 0, 0 };
		q_body.unit(0, 0, 0, 1);
	}

	bool process(const BinsFrame& frame);
	const Orientation& get_orientation();
};

#endif /* PROCESS_PROCESSER_H_ */
