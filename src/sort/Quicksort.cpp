/*
 * Quicksort.cpp
 *
 *  Created on: Mar 5, 2014
 *      Author: lizardo
 */

#include "Quicksort.h"

bool Quicksort::comp(Record & a, Record & b) {
	if (a.getTerm() < b.getTerm())
		return true;
	else if (a.getTerm() == b.getTerm() && a.getDocument() < b.getDocument())
		return true;
	else
		return false;
}

void Quicksort::sort(vector<Record> & arr) {
	this->qsort(arr, 0, arr.size() - 1);
}

void Quicksort::qsort(vector<Record> & arr, int left, int right) {

	int i = left, j = right;
	Record tmp;
	Record pivot = arr[(left + right) / 2];

	/* partition */
	while (i <= j) {
		while (this->comp(arr[i], pivot) == true)
			i++;
		while (this->comp(pivot, arr[j]) == true)
			j--;
		if (i <= j) {
			tmp = arr[i];
			arr[i] = arr[j];
			arr[j] = tmp;
			i++;
			j--;
		}
	}

	/* recursion */
	if (left < j)
		qsort(arr, left, j);
	if (i < right)
		qsort(arr, i, right);
}

Quicksort::Quicksort() {
}
