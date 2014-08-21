/*
 * ExternalMergeSort.h
 *
 *  Created on: Feb 28, 2014
 *      Author: lizardo
 */

#ifndef EXTERNALMERGESORT_H_
#define EXTERNALMERGESORT_H_

#include <string>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <list>
#include "../commons/Record.h"
#include "../sort/Quicksort.h"
#include "../utils/TimeProfiler.h"

using namespace std;

class ExternalMergeSort {
private:
	int ramSize;
	string directory;

	TimeProfiler timep;
	TimeProfiler timeIO;
	double loadTime;
	double storeTime;
	double quickSortTime;
	double mergeTime;

	int loadRecords(FILE *fp, vector<Record> & block, int maxRecords);
	string storeRecords(vector<Record> & block, int numRecords);
	void merge(list<string> &blocksFilesNames);
	void merge2files(FILE *fp1, FILE *fp2, FILE *fpOut);
	int readRecord(FILE *fp, Record &record);
	void printRecord(FILE *fp, Record &record);

public:
	ExternalMergeSort(int ramSize, string directory, string unorderedFilePath);
};

#endif /* EXTERNALMERGESORT_H_ */
