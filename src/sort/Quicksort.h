/*
 * Quicksort.h
 *
 *  Created on: Mar 5, 2014
 *      Author: lizardo
 */

#ifndef QUICKSORT_H_
#define QUICKSORT_H_

#include <vector>

#include "../commons/Record.h"

using namespace std;

class Quicksort {
private:
	bool comp(Record & a, Record & b);

	void qsort(vector<Record> & arr, int left, int right);

public:
	Quicksort();

	void sort(vector<Record>& arr);
};

#endif /* QUICKSORT_H_ */
