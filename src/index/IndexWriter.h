/*
 * IndexWriter.h
 *
 *  Created on: Mar 11, 2014
 *      Author: lizardo
 */

#ifndef INDEXWRITER_H_
#define INDEXWRITER_H_

#include "IndexConfig.h"

#include "../commons/DocInfo.h"
#include "../commons/Record.h"
#include "../index/IndexConfig.h"
#include "../utils/FileManager.h"
#include "../utils/DocFileManager.h"

#include <list>
#include <unordered_map>
#include <string>
#include <vector>

using namespace std;

/*
 * This class parses the collection, extracts the terms and creates the
 * temporary file partially sorted.
 */
class IndexWriter {
private:

	/*
	 * Contains some configuration stuff.
	 */
	IndexConfig config;

	/*
	 * Dictionary. It stores the term and its uint code.
	 */
	unordered_map<string, unsigned int> dictionary;

	unordered_map<string, unsigned int> urlDictionary;

	FILE *fpLinks;

	/*
	 * Buffer to store the parsed triples in memory before storing in disk.
	 */
	vector<Record> buffer;

	/*
	 * Buffer max size. It is calculated based on the size of memory set up in
	 * the IndexConfig.
	 */
	size_t buffer_max_size;

	/*
	 * Block max size. Maximum number of records that can be stored in a block.
	 */
	size_t block_max_size;

	/*
	 * Count the number of different terms parsed.
	 */
	size_t term_counter;

	/*
	 * This object is responsible for managing the file.
	 */
	FileManager fileManager;

	/*
	 * This object is responsible for managing the file.
	 */
	DocFileManager *docFileManager;

	/*
	 * Add a new document to the index file. This function tokenize the document
	 * and extracts its term.
	 */
	void addDocument(DocInfo &doc, size_t doc_counter);

	/*
	 * Calculate the term frequency inside a document.
	 */
	void countTermFrequency(
			unordered_map<unsigned int, unsigned int> & termFrequency,
			string content);

	/*
	 * Save the buffer in disk.
	 */
	void flushBuffer();

	void processAnchors(size_t &document_counter);

	void removeDuplicates(vector<Record> &records);

	void saveLinks(unsigned int docId, list<pair<string, string> > links);

	void saveInLinks(unordered_map<unsigned int, list<unsigned int> > &inLinks);

	void saveOutLinks(unordered_map<unsigned int, unsigned int > &outLinks);

public:

	/*
	 * Constructor.
	 */
	IndexWriter(IndexConfig &config);

	/*
	 * Destructor. It prints the statistics.
	 */
	~IndexWriter();

	/*
	 * Parses a collection of documents and builds a file containing the
	 * triples.
	 */
	void writer(string &collection_directory, string &collection_index);
};

#endif /* INDEXWRITER_H_ */
