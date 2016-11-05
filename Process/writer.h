/*
 * writer.h
 *
 *  Created on: Nov 5, 2016
 *      Author: kar
 */

#ifndef PROCESS_WRITER_H_
#define PROCESS_WRITER_H_

#include <fcntl.h>
#include <unistd.h>
#include <sys/uio.h>

#include "processer.h"

int CreateWriter(const char *name);
void WriteToFile(int fd, const struct GyroResult *data);
int CloseWriter(int fd);

#endif /* PROCESS_WRITER_H_ */
