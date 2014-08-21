/*
 * TimeProfiler.cpp
 *
 *  Created on: Mar 8, 2014
 *      Author: lizardo
 */

#include "TimeProfiler.h"

double TimeProfiler::calcTime() {
	double utime, stime;
	struct rusage resources;
	int rc;

	if ((rc = getrusage(RUSAGE_SELF, &resources)) != 0)
		perror("getrusage failed");

	utime = (double) resources.ru_utime.tv_sec
			+ 1.e-6 * (double) resources.ru_utime.tv_usec;
	stime = (double) resources.ru_stime.tv_sec
			+ 1.e-6 * (double) resources.ru_stime.tv_usec;

	return utime + stime;
}

void TimeProfiler::start() {
	this->startStamp = calcTime();
}

double TimeProfiler::reportTime() {
	return calcTime() - startStamp;
}
