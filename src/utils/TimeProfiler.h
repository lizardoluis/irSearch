/*
 * TimeProfiler.h
 *
 *  Created on: Mar 8, 2014
 *      Author: lizardo
 */

#ifndef TIMEPROFILER_H_
#define TIMEPROFILER_H_

#include <cstdio>
#include <sys/resource.h>
#include <sys/time.h>

class TimeProfiler {
private:
	double startStamp;

	double calcTime();

public:

	void start();

	double reportTime();
};

#endif /* TIMEPROFILER_H_ */

