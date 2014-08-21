/*
 * Heap.cpp
 *
 *  Created on: Mar 3, 2014
 *      Author: lizardo
 */

#include "Heap.h"

Heap::Heap() {
}

void Heap::push(Record record, size_t blockNumber) {
	heap.push(make_pair(record, blockNumber));
}

void Heap::top(Record &record, size_t &block_number){

	pair<Record, size_t> top = heap.top();

	heap.pop();

	record = top.first;
	block_number = top.second;
}

bool Heap::empty() {
	return heap.empty();
}
