/*
 * EliasGamma.cpp
 *
 *  Created on: Mar 17, 2014
 *      Author: lizardo
 */

#include "EliasGamma.h"

#include <cmath>
#include <cstdlib>
#include <cstdio>


void EliasGamma::push_bit(vector<unsigned char> &bitvector, unsigned int &idx, unsigned int bit){

	if(bitvector.empty()){
		bitvector.push_back(0);
	}

	if(idx == 8){
		idx = 0;
		bitvector.push_back(0);
	}

	size_t curr = bitvector.size()-1;

	if(bit == 1)
		bitvector[curr] |= (128 >> idx);

	idx++;
}

unsigned int EliasGamma::pop_bit(vector<unsigned char> &bitvector, unsigned int idx){
	unsigned int i = idx / 8;
	unsigned int j = idx % 8;
	unsigned char mask = 1 << (7-j);
	return (bitvector[i] & mask) >> (7-j);
}

void EliasGamma::encode(vector<unsigned int> &numbers, vector<unsigned char> &bitvector) {

	unsigned int idx = 0;

	for(size_t i=0; i < numbers.size(); i++){

		// Increments to code 0
		unsigned int num = numbers[i]+1;

		// First part
		unsigned int un = floor(log2(num));
		for (unsigned int i = 0; i < un; ++i) {
			push_bit(bitvector, idx, 0);
		}
		push_bit(bitvector, idx, 1);

		// Second part
		unsigned int x = num - pow(2, un);
		for (unsigned int i = 0; i < un; ++i) {
			push_bit(bitvector, idx, (x >> (un - i -1)) & 1);
		}
	}
}

void EliasGamma::decode(vector<unsigned char> &bitvector, vector<unsigned int> &numbers) {

	unsigned int size = bitvector.size()*8;

	unsigned int k=0;

	while(k < size){

		unsigned int cu = 0;

		// Extract the fist part
		while (pop_bit(bitvector, k) == 0 && k < size) {
			cu++;
			k++;
		}

		if(k == size)
			break;

		k++;

		// Extract the second part
		unsigned int cb = 0;
		if(cu){
			for (unsigned int i = 0; i < cu; i++) {
				unsigned int bit = pop_bit(bitvector, k++);
				cb = cb << 1;
				cb = cb | bit;
			}
		}

		numbers.push_back(pow(2, cu) + cb - 1);
	}
}
