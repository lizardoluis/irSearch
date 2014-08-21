/*
 * DictManager.cpp
 *
 *  Created on: Mar 12, 2014
 *      Author: lizardo
 */

#include "DictFileManager.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

void DictFileManager::save_dictionary(string filePath,
		unordered_map<string, unsigned int>& dictionary) {

	FILE *fp = fopen(filePath.c_str(), "w");
	if (fp == NULL) {
		printf("Error when opening dictionary file.");
		exit(0);
	}

	fprintf(fp, "%ld\n", dictionary.size());

	for (unordered_map<string, unsigned int>::iterator it = dictionary.begin();
			it != dictionary.end(); ++it) {

		fprintf(fp, "%s %d\n", it->first.c_str(), it->second);
	}

	fclose(fp);
}

size_t DictFileManager::read_dictionary(char*** dict, string filePath) {

	size_t number_of_terms;

	// Open the dictionary file
	FILE *fp = fopen(filePath.c_str(), "r");
	if (fp == NULL) {
		printf("Error when opening dictionary file.");
		exit(0);
	}

	// Read the number of terms
	int n = fscanf(fp, "%lu\n", &number_of_terms);
	if (n != 1) {
		printf("Dictionary file invalid.");
		exit(0);
	}

	// Allocate vector capacity to the store the quantity of terms
	*dict = (char**) malloc(number_of_terms * sizeof(char*));
	if (dict == NULL) {
		printf("Dictionary memory allocation error.");
		exit(0);
	}

	// Parse the dictionary file and copy the terms to an array
	char str[51];
	unsigned int id;
	while (fscanf(fp, "%51s %u\n", str, &id) > 0) {
		(*dict)[id - 1] = (char *) malloc(strlen(str) + 1);
		strcpy((*dict)[id - 1], str);
	}

	fclose(fp);

	return number_of_terms;
}
