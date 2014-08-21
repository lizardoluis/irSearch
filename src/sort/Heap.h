/*
 * Heap.h
 *
 *  Created on: Mar 3, 2014
 *      Author: lizardo
 */

#ifndef HEAP_H_
#define HEAP_H_

#include <algorithm>
#include <queue>
#include <vector>
#include <utility>

#include "../commons/Record.h"

using namespace std;

struct Less {
	bool operator()(const pair<Record, size_t> a,
			const pair<Record, size_t> b) const {
		if (a.first.getTerm() > b.first.getTerm())
			return true;
		else if (a.first.getTerm() == b.first.getTerm()
				&& a.first.getDocument() > b.first.getDocument())
			return true;
		else
			return false;
	}
};

class Heap {
private:

	priority_queue < pair<Record, size_t>, vector<pair<Record, size_t> >, Less> heap;

public:
	Heap();

	void push(Record record, size_t blockNumber);

	void top(Record &record, size_t &block_number);

	bool empty();
};

#endif /* HEAP_H_ */
