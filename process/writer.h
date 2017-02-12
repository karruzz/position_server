/*
 * writer.h
 *
 *  Created on: Feb 12 2017
 *  Copyright © 2017-2080 Ilja Karasev. All rights reserved.
 *     License: GNU GPL 3
 */

#ifndef PROCESS_WRITER_H_
#define PROCESS_WRITER_H_

#include "processer.h"

#include <fstream>
#include <cstdio>

class Writer {
	std::ofstream ofile;
public:
	~Writer() { close(); }
	void open(const std::string& name);
	void write(const Orientation& data);
	void close();
};

#endif /* PROCESS_WRITER_H_ */
