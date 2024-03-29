/*
 * writer.cpp
 *
 *  Created on: Feb 12, 2017
 *  Copyright © 2017-2080 Ilja Karasev. All rights reserved.
 *     License: GNU GPL 3
 */

#include "writer.h"

void Writer::open(const std::string& name)
{
	ofile = std::ofstream(name, std::ios::binary);
	if (!ofile.is_open())
		throw new std::runtime_error("cannot create out file");
}

void Writer::write(const Orientation& data)
{
	ofile.write(reinterpret_cast<const char*>(&data), sizeof(Orientation));
	ofile.flush();
}

void Writer::close()
{
	ofile.close();
}


