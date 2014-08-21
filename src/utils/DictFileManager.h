/*
 * DictFileManager.h
 *
 *  Created on: Mar 12, 2014
 *      Author: lizardo
 */

#ifndef DICTMANAGER_H_
#define DICTMANAGER_H_

#include <unordered_map>
#include <string>

using namespace std;

class DictFileManager {
public:

	DictFileManager() {
	}

	void save_dictionary(string filePath, unordered_map<string, unsigned int> &dictionary);

	size_t read_dictionary(char*** dict, string filePath);
};

#endif /* DICTMANAGER_H_ */
