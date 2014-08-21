/*
 * Index.cpp
 *
 *  Created on: Apr 1, 2014
 *      Author: lizardo
 */

#include "Index.h"
#include "../compression/EliasGamma.h"
#include "../utils/DictFileManager.h"
#include "../utils/TimeProfiler.h"

#include <cmath>
#include <list>
#include <iostream>

map<string, double> iReportedTime;

TimeProfiler tTotal;
TimeProfiler buildIndex;
TimeProfiler buildHash;
TimeProfiler gammaTime;
TimeProfiler flushTime;

Index::Index(IndexConfig &config) {

	tTotal.start();

	char** dict = NULL;
	DictFileManager dictFileManager;

	// Read dictionary file and get the terms in a vector ordered by its code
	int numberOfTerms = dictFileManager.read_dictionary(&dict,
			config.getDictFilePath());

	// Create the cmph
	buildHash.start();
	hash = new Hash(dict, numberOfTerms, config.getDirectory() + "temp.mph");
	iReportedTime["hashTime"] = buildHash.reportTime();

	// Create lexicon
	lexicon = new Lexicon(hash->getSize());

	// Open an index file
	indexFile = fopen(config.getIndexFilePath().c_str(), "wb+");
	if (indexFile == NULL) {
		printf("Error when opening index file.");
		exit(0);
	}

	number_of_documents = config.getDocumentsMaxNumber();

	// Parse triples and build the index file
	buildIndex.start();
	parseTriplesFile(dict, config.getTmpFilePath());
	iReportedTime["indexTime"] = buildIndex.reportTime();

	// Dict is not necessary anymore
	free(dict);
}

Index::~Index() {
	delete hash;
	delete lexicon;
	fclose(indexFile);

	iReportedTime["timeTotal"] = tTotal.reportTime();

	for (map<string, double>::iterator it = iReportedTime.begin();
			it != iReportedTime.end(); ++it) {
		printf("%s\t%g\n", it->first.c_str(), it->second);
	}
}

// Compressed
void Index::parseTriplesFile(char** dict, string filePath) {

	// Open the triples file
	FILE *triplesFile = fopen(filePath.c_str(), "rb");
	if (triplesFile == NULL) {
		printf("Error when opening triples file.");
		exit(0);
	}

	EliasGamma gamma;
	vector<unsigned int> serial_buffer;
	size_t pos = 0;
	unsigned int term, doc, freq, last_doc = 0, cterm = 0, docCounter = 0;

	list<pair<unsigned int, unsigned int> > docList;

	avg_doclen = 0;

	while (!feof(triplesFile)) {

		fread(&term, sizeof(unsigned int), 1, triplesFile);
		fread(&doc, sizeof(unsigned int), 1, triplesFile);
		fread(&freq, sizeof(unsigned int), 1, triplesFile);

		if (cterm == 0) {
			cterm = term;
		}

		if (cterm != term || feof(triplesFile)) {

			// Get its hash position
			size_t p = hash->search(dict[cterm - 1]);

			// Compress buffer
			vector<unsigned char> compressed_data;
			gammaTime.start();
			gamma.encode(serial_buffer, compressed_data);
			iReportedTime["compressionTime"] += gammaTime.reportTime();
			serial_buffer.clear();
			last_doc = 0;

			// Record the current file position for the new term
			pos = ftell(indexFile);

			// Calculate tf-idf
			double idf = log2(
					(double) number_of_documents / (double) docCounter);
			//			printf("%u %u %g\n", cterm, docCounter, idf);

			// Adds the entry to the lexicon
			lexicon->add(p, dict[cterm - 1], docList.size(), pos, idf);

			while (!docList.empty()) {

				flushTime.start();
				fwrite(&docList.front().first, sizeof(unsigned int), 1,
						indexFile);
				fwrite(&docList.front().second, sizeof(unsigned int), 1,
						indexFile);
				iReportedTime["flushTime"] += flushTime.reportTime();

				documentNorm[docList.front().first] += pow(
						(1 + log2(docList.front().second)) * idf, 2);

				documentLen[docList.front().first] += docList.front().second;

				avg_doclen += docList.front().second;

				docList.pop_front();
			}

			docCounter = 0;
			docList.clear();

			// Flush to the index file
			flushTime.start();
			for (size_t i = 0; i < compressed_data.size(); i++) {
				fputc(compressed_data[i], indexFile);
			}
			iReportedTime["flushTime"] += flushTime.reportTime();

			// Updates the cterm and ndoc for the following dict being read
			cterm = term;
		}

		docList.push_back(make_pair(doc, freq));
		docCounter++;

		serial_buffer.push_back(doc - last_doc);
		serial_buffer.push_back(freq);

		last_doc = doc;
	}

	avg_doclen /= number_of_documents;

	fclose(triplesFile);
}

// Not compressed
/*void Index::parseTriplesFile(char** dict, string filePath) {

 // Open the triples file
 FILE *triplesFile = fopen(filePath.c_str(), "rb");
 if (triplesFile == NULL) {
 printf("Error when opening triples file.");
 exit(0);
 }

 EliasGamma gamma;
 vector<unsigned int> serial_buffer;
 size_t pos = 0;
 unsigned int term, doc, freq, last_doc = 0, cterm = 0, docCounter = 0;

 list<pair<unsigned int, unsigned int> > docList;

 avg_doclen = 0;

 while (!feof(triplesFile)) {

 fread(&term, sizeof(unsigned int), 1, triplesFile);
 fread(&doc, sizeof(unsigned int), 1, triplesFile);
 fread(&freq, sizeof(unsigned int), 1, triplesFile);

 if (cterm == 0) {
 cterm = term;
 }

 if (cterm != term || feof(triplesFile)) {

 // Get its hash position
 size_t p = hash->search(dict[cterm - 1]);

 // Compress buffer
 vector<unsigned char> compressed_data;
 gammaTime.start();
 gamma.encode(serial_buffer, compressed_data);
 iReportedTime["compressionTime"] += gammaTime.reportTime();
 serial_buffer.clear();
 last_doc = 0;

 // Record the current file position for the new term
 pos = ftell(indexFile);

 // Calculate tf-idf
 double idf = log2((double) number_of_documents / (double) docCounter);
 //			printf("%u %u %g\n", cterm, docCounter, idf);

 while (!docList.empty()) {
 documentNorm[docList.front().first] += pow(
 (1 + log2(docList.front().second)) * idf, 2);

 documentLen[docList.front().first] += docList.front().second;

 avg_doclen += docList.front().second;

 docList.pop_front();
 }

 docCounter = 0;
 docList.clear();

 // Adds the entry to the lexicon
 lexicon->add(p, dict[cterm - 1], compressed_data.size(), pos, idf);

 // Flush to the index file
 flushTime.start();
 for (size_t i = 0; i < compressed_data.size(); i++) {
 fputc(compressed_data[i], indexFile);
 }
 iReportedTime["flushTime"] += flushTime.reportTime();

 // Updates the cterm and ndoc for the following dict being read
 cterm = term;
 }

 docList.push_back(make_pair(doc, freq));
 docCounter++;

 serial_buffer.push_back(doc - last_doc);
 serial_buffer.push_back(freq);

 last_doc = doc;
 }

 avg_doclen /= number_of_documents;

 fclose(triplesFile);
 }*/

// Compressed
void Index::getInvertedList(size_t pos, size_t size,
		vector<term_info>& inverted_list) {

	EliasGamma gamma;

	inverted_list.clear();

	size_t err = fseek(indexFile, pos, SEEK_SET);
	if (err != 0) {
		printf("Error finding inverted index location.");
		exit(0);
	}

	// Read index file and uncompress
	vector<unsigned char> compressed(size);

	for (size_t i = 0; i < size; i++) {

		unsigned int doc, freq;

		term_info info;
		fread(&doc, sizeof(unsigned int), 1, indexFile);
		fread(&freq, sizeof(unsigned int), 1, indexFile);

		info.doc = doc;
		info.freq = freq;
		inverted_list.push_back(info);
	}

	vector<unsigned int> serial_buffer;

	gamma.decode(compressed, serial_buffer);

	term_info info;
	for (size_t i = 0; i < serial_buffer.size(); i += 2) {
		if (i == 0) {
			info.doc = serial_buffer[i];
		} else {
			info.doc = inverted_list[inverted_list.size() - 1].doc
					+ serial_buffer[i];
		}
		info.freq = serial_buffer[i + 1];

		inverted_list.push_back(info);
	}
}

// Not compressed
/*void Index::getInvertedList(size_t pos, size_t size,
 vector<term_info>& inverted_list) {

 EliasGamma gamma;

 inverted_list.clear();

 size_t err = fseek(indexFile, pos, SEEK_SET);
 if (err != 0) {
 printf("Error finding inverted index location.");
 exit(0);
 }

 // Read index file and uncompress
 vector<unsigned char> compressed(size);

 for (size_t i = 0; i < size; i++) {
 compressed[i] = fgetc(indexFile);
 }

 vector<unsigned int> serial_buffer;

 gamma.decode(compressed, serial_buffer);

 term_info info;
 for (size_t i = 0; i < serial_buffer.size(); i += 2) {
 if (i == 0) {
 info.doc = serial_buffer[i];
 } else {
 info.doc = inverted_list[inverted_list.size() - 1].doc
 + serial_buffer[i];
 }
 info.freq = serial_buffer[i + 1];

 inverted_list.push_back(info);
 }
 }*/

unsigned int Index::searchTerm(string term, vector<term_info>& inverted_list) {

	// Apply hash function
	size_t p = hash->search(term);

	// Checks weather term exists in lexicon
	if (lexicon->getTerm(p) != NULL && term.compare(lexicon->getTerm(p)) != 0) {
		return 0;
	}

	getInvertedList(lexicon->getPos(p), lexicon->getSize(p), inverted_list);

//	cout << term << endl;
//	for(term_info t : inverted_list){
//		cout << t.doc << " " << t.freq << endl;
//	}

	return inverted_list.size();
}

unsigned int Index::getNumberOfDocuments() {
	return number_of_documents;
}

double Index::getTermIDF(string term) {
	size_t p = hash->search(term);
	return lexicon->getIdf(p);
}

double Index::getDocumentNorm(unsigned int doc) {
	return sqrt(documentNorm[doc]);
}

unsigned int Index::getDocumentLen(unsigned int doc) {
	return documentLen[doc];
}

double Index::getAvgDocLen() {
	return avg_doclen;
}
