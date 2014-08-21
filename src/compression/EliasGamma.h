/*
 * EliasGamma.h
 *
 *  Created on: Mar 17, 2014
 *      Author: lizardo
 */

#ifndef ELIASGAMMA_H_
#define ELIASGAMMA_H_

#include <vector>

using namespace std;

class EliasGamma {
private:
	void push_bit(vector<unsigned char> &bitvector, unsigned int &idx, unsigned int bit);

	unsigned int pop_bit(vector<unsigned char> &bitvector, unsigned int idx);

public:
	/*
	 * Encoder function.
	 */
	void encode(vector<unsigned int> &numbers, vector<unsigned char> &bitvector);

	/*
	 * Decoder function.
	 */
	void decode(vector<unsigned char> &bitvector, vector<unsigned int> &numbers);
};

#endif /* ELIASGAMMA_H_ */
