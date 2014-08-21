/*
 * hash.h
 *
 *  Created on: Mar 9, 2014
 *      Author: lizardo
 */

#ifndef CMPH_H_
#define CMPH_H_

#include <cmph.h>
#include <cstdio>
#include <string>
#include <cstring>

using namespace std;

class Hash {
private:

	cmph_t *cmph;

	FILE* mphf_fd;

public:

	Hash(char **keys, unsigned int nkeys, string mphPath);

	~Hash();

	size_t search(string key);

	size_t getSize();
};

#endif /* HASH_H_ */
