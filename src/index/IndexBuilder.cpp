///*
// * IndexBuilder.cpp
// *
// *  Created on: Mar 15, 2014
// *      Author: lizardo
// */
//
//#include "IndexBuilder.h"
//#include "../analysis/Tokenizer.h"
//#include "../compression/EliasGamma.h"
//#include "../utils/TimeProfiler.h"
//
//#include <list>
//
//#define AND 0
//#define OR 1
//
//typedef short op;
//
//map<string, size_t> bReportedData;
//map<string, double> bReportedTime;
//
//TimeProfiler timeBuild;
//TimeProfiler timeQuery;
//
//
//Index* IndexBuilder::buildIndex(IndexConfig &config){
//
//	string triplesPath = config.getTmpFilePath();
//	string dictionaryPath = config.getDictFilePath();
//	string indexPath = config.getIndexFilePath();
//
//	char** dict = NULL;
//
//	// Read dictionary file and get the terms in a vector ordered by its
//	// code
//	DictFileManager dictFileManager;
//	int numberOfTerms = dictFileManager.read_dictionary(&dict, dictionaryPath);
//
//	// Create the cmph
//	Hash hash(dict, numberOfTerms, config.getDirectory() + "temp.mph");
//
//	// Create lexicon
//	Lexicon lexicon(hash->getSize());
//
//	// Parse triples and build the index file
//	parseTriples(lexicon, hash, dict, triplesPath, indexPath);
//
//	// Dict is not necessary anymore
//	free(dict);
//
//	Index *index = new Index();
//	index->setHash(hash);
//	index->setLexicon(lexicon);
//	index->setIndexFile();
//}
//
//
//IndexBuilder::~IndexBuilder() {
//	delete hash;
//
//	for (map<string, double>::iterator it = bReportedTime.begin();
//			it != bReportedTime.end(); ++it) {
//		printf("%s\t%g\n", it->first.c_str(), it->second);
//	}
//
//	for (map<string, size_t>::iterator it = bReportedData.begin();
//			it != bReportedData.end(); ++it) {
//		printf("%s\t%ld\n", it->first.c_str(), it->second);
//	}
//}
//
//
////void IndexBuilder::build(){
////
////	string triplesPath = config.getTmpFilePath();
////	string dictionaryPath = config.getDictFilePath();
////	string indexPath = config.getIndexFilePath();
////
////	char** dict = NULL;
////
////	// Read dictionary file and get the terms in a vector ordered by its
////	// code
////	DictFileManager dictFileManager;
////	int numberOfTerms = dictFileManager.read_dictionary(&dict, dictionaryPath);
////
////	// Create the cmph
////	hash = new Hash(dict, numberOfTerms, config.getDirectory() + "temp.mph");
////
////	printf("number of terms %d\n", numberOfTerms);
////
////	// Lexicon containing
////	Lexicon lexicon(hash->getSize());
////
////	// Parse triples and build the index file
////	parseTriples(lexicon, hash, dict, triplesPath, indexPath);
////
////	// Dict is not necessary anymore
////	free(dict);
////}
//
//void IndexBuilder::parseTriples(Lexicon &lexicon, Hash *hash, char** dict, string triplesPath, string indexPath) {
//
//	// Open the triples file
//	FILE *fpTriples = fopen(triplesPath.c_str(), "rb");
//	if (fpTriples == NULL) {
//		printf("Error when opening triples file.");
//		exit(0);
//	}
//
//	// Creates an index file
//	FILE *fpIndex = fopen(indexPath.c_str(), "wb");
//	if (fpIndex == NULL) {
//		printf("Error when opening index file.");
//		exit(0);
//	}
//
//	EliasGamma gamma;
//
//	// Used to store the data before being compressed.
//	vector <unsigned int> serial_buffer;
//
//	unsigned int term, doc, freq, last_doc = 0, cterm = 0;
//	size_t pos = 0;
//
//	while (!feof(fpTriples)) {
//
//		fread(&term, sizeof(unsigned int), 1, fpTriples);
//		fread(&doc, sizeof(unsigned int), 1, fpTriples);
//		fread(&freq, sizeof(unsigned int), 1, fpTriples);
//
//		if(cterm == 0){
//			cterm = term;
//		}
//
//		if( cterm != term ){
//
//			// Get its hash position
//			size_t p = hash->search(dict[cterm-1]);
//
//			// Compress buffer
//			vector<unsigned char> compressed_data;
//			gamma.encode(serial_buffer, compressed_data);
//			serial_buffer.clear();
//			last_doc = 0;
//
//			// Record the current file position for the new term
//			pos = ftell(fpIndex);
//
//			// Adds the entry to the lexicon
//			lexicon.add(p, dict[cterm-1], compressed_data.size(), pos);
//
//			// Flush to the index file
//			for(size_t i=0; i<compressed_data.size(); i++){
//				fputc(compressed_data[i], fpIndex);
//			}
//
//			// Updates the cterm and ndoc for the following dict being read
//			cterm = term;
//		}
//
//		serial_buffer.push_back(doc-last_doc);
//		serial_buffer.push_back(freq);
//
//		last_doc = doc;
//	}
//
//	bReportedData["lexicon_size"] = lexicon.getLexSize();
//	bReportedData["index_file_size"] = ftell(fpIndex);
//
//	fclose (fpTriples);
//	fclose (fpIndex);
//}
