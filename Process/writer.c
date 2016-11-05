/*
 * writer.c
 *
 *  Created on: Nov 5, 2016
 *      Author: kar
 */

#include "writer.h"

int CreateWriter(const char *name)
{
	return open(name, O_RDWR | O_CREAT | O_TRUNC );
}

static const int dataSize = sizeof(struct GyroResult);
void WriteToFile(int fd, const struct GyroResult *data)
{
	char buffer[dataSize];
	struct iovec iov[1];

	struct GyroResult *result = (struct GyroResult *)(buffer);
	*result = *data;

	iov[0].iov_base = buffer;
	iov[0].iov_len = dataSize;

	writev(fd, iov, 1);
}

int CloseWriter(int fd)
{
	return close(fd);
}

