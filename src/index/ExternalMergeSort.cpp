/*
 * ExternalMergeSort.cpp
 *
 *  Created on: Feb 28, 2014
 *      Author: lizardo
 */

#include "ExternalMergeSort.h"

ExternalMergeSort::ExternalMergeSort(int ramSize, string directory, string unorderedFilePath) {

	loadTime = 0;
	storeTime = 0;
	quickSortTime = 0;
	mergeTime = 0;


	this->ramSize = ramSize;
	this->directory = directory;

	//	int numBlocks = 0;
	int blockSize = ramSize/sizeof(Record); //TODO: ramSize - lexicon size

	vector<Record> block(blockSize);
	list<string> blocksFilesNames;

	// Opens the unordered file to read.
	FILE *unorderedFile = fopen(unorderedFilePath.c_str(), "r");
	if (!unorderedFile) {
		printf("\nError when opening the temporary file");
		exit(0);
	}

	int recordsLoaded = 0;
	Quicksort quicksort;
	do {
		// Reads a block of records from the unordered file
		recordsLoaded = loadRecords(unorderedFile, block, blockSize);

		// Sorts it
		timep.start();
		quicksort.sort(block);
		quickSortTime += timep.reportTime();
		//		sort(block.begin(), block.end(), recordComp2);

		// Writes the sorted block into a temporary file and stores the path of
		// the temporary file into the array of blocks names.
		blocksFilesNames.push_back(storeRecords(block, recordsLoaded));

	} while (recordsLoaded == blockSize);

	// Closes the temporary file;
	fclose(unorderedFile);

	// MergeFiles
	timep.start();
	merge(blocksFilesNames);
	mergeTime += timep.reportTime();


	cout << "Time EMload: " << loadTime << endl;
	cout << "Time EMstore: " << storeTime << endl;
	cout << "Time EMquicksort: " << quickSortTime << endl;
	cout << "Time EMmerge: " << mergeTime << endl;


	//TODO: remove temporary file
	//	remove(unorderedFilePath);
	//TODO: return unorderedFile name
}

int ExternalMergeSort::loadRecords(FILE* fp, vector<Record>& block,
		int maxRecords) {

	timeIO.start();

	block.clear();

	int i=0,n=1;
	while(i < maxRecords && n > 0) {
		int term, document;
		short frequency;
		n = fscanf(fp, "%d;%d;%hd\n", &term, &document, &frequency);
		Record record(term, document, frequency);
		block.push_back(record);
		++i;
	}

	loadTime += timeIO.reportTime();

	return i;
}

string ExternalMergeSort::storeRecords(vector<Record>& block, int numRecords) {

	timeIO.start();

	static unsigned int n=0;

	// creates a temporary file;
	char intStr[21];
	sprintf(intStr, "%d", n++);
	//TODO: consider directory
	string fileName = "tmp" + string(intStr) + ".txt";

	FILE *fp = fopen(fileName.c_str(),"w");
	if (!fp) {
		printf("\nError when opening the file");
		exit(0);
	}

	for(int i=0; i<numRecords; i++) {
		Record record = block[i];
		fprintf(fp, "%d;%d;%hd\n", record.getTerm(), record.getDocument(), record.getFrequency());
	}

	fclose(fp);

	storeTime += timeIO.reportTime();

	return fileName;
}

void ExternalMergeSort::merge(list<string> &blocksFilesNames) {

	int outputCounter = 0;

	while(blocksFilesNames.size() > 1){

		//		// Increments number of blocks counter
		//		if(recordsLoaded > 0){
		//			numBlocks++;
		//		}


		string fname1 = blocksFilesNames.front();
		blocksFilesNames.pop_front();
		string fname2 = blocksFilesNames.front();
		blocksFilesNames.pop_front();

		char intStr[21];
		sprintf(intStr, "%d", outputCounter++);
		//TODO: consider directory
		string outFileName = "out" + string(intStr) + ".txt";
		if(blocksFilesNames.empty()){
			outFileName = "orderedFile.txt";
		}

		FILE *fp1 = fopen(fname1.c_str(), "r");
		FILE *fp2 = fopen(fname2.c_str(), "r");
		FILE *fpOut = fopen(outFileName.c_str(), "w");

		merge2files(fp1, fp2, fpOut);

		fclose(fp1);
		fclose(fp2);
		fclose(fpOut);

		remove(fname1.c_str());
		remove(fname2.c_str());

		blocksFilesNames.push_back(outFileName);
	}

	//TODO: return the merged file name
}

void ExternalMergeSort::merge2files(FILE *fp1, FILE *fp2, FILE *fpOut) {
	int n1, n2;
	Record r1, r2;

	n1 = readRecord(fp1, r1);
	n2 = readRecord(fp2, r2);

	// Merge loop
	while(n1>0 && n2>0){
		if(r1.getTerm() < r2.getTerm() || (r1.getTerm() == r2.getTerm() && r1.getDocument() == r2.getDocument())){
			printRecord(fpOut, r1);
			n1 = readRecord(fp1, r1);
		}
		else{
			printRecord(fpOut, r2);
			n2 = readRecord(fp2, r2);
		}
	}

	// Handles run outs of data, in either file
	if (n1 > 0 && n2 < 1) {
		while (n1 > 0) {
			printRecord(fpOut, r1);
			n1 = readRecord(fp1, r1);
		}
	} else if (n2 > 0 && n1 < 1) {
		while (n2 > 0) {
			printRecord(fpOut, r2);
			n2 = readRecord(fp2, r2);
		}
	}
}

int ExternalMergeSort::readRecord(FILE *fp, Record &record){

	timeIO.start();

	int term, document;
	short frequency;
	int n = fscanf(fp, "%d;%d;%hd\n", &term, &document, &frequency);
	record.setTerm(term);
	record.setDocument(document);
	record.setFrequency(frequency);

	loadTime += timeIO.reportTime();

	return n;
}

void ExternalMergeSort::printRecord(FILE *fp, Record &record){
	fprintf(fp, "%d;%d;%hd\n", record.getTerm(), record.getDocument(), record.getFrequency());
}

