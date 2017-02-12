/*
 * log.h
 *
 *  Created on: Feb 04, 2017
 *     Author: karruzz
 */

#ifndef LOG_H_
#define LOG_H_

#include <iostream>
#include <sstream>

namespace Utils {

class Log
{
public:
	const std::string prefix;

	Log(const std::string& prefix_) : prefix(prefix_) {
		uncaught = std::uncaught_exceptions();
	}

	~Log() {
		if (uncaught >= std::uncaught_exceptions()) // todo: ???
			std::cout << prefix << stream.str();
	}

	std::stringstream stream;
	int uncaught;
};

class LogInfo : public Log
{
public:
	LogInfo() : Log("[INFO] ") {}
};

class LogWarning : public Log
{
public:
	LogWarning() : Log("[WARN] ") {}
};

class LogError : public Log
{
public:
	LogError() : Log("[ERROR] ") {}
};

template <typename T>
Log& operator<<(Log& record, T&& t) {
	record.stream << std::forward<T>(t);
	return record;
}

template <typename T>
Log& operator<<(Log&& record, T&& t) {
	return record << std::forward<T>(t);
}

}

#endif /* LOG_H_ */
