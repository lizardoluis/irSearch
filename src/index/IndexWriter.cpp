/*
 * IndexWriter.cpp
 *
 *  Created on: Mar 11, 2014
 *      Author: lizardo
 */

#include "IndexWriter.h"

#include <CollectionReader.h>
#include <Document.h>
#include <map>

#include <iostream>

#include "IndexConfig.h"

#include "../analysis/Tokenizer.h"
#include "../commons/DocInfo.h"
#include "../parser/HTMLParser.h"
#include "../sort/Quicksort.h"
#include "../utils/DictFileManager.h"
#include "../utils/TimeProfiler.h"
#include "../compression/md5.h"

// To statistics
map<string, size_t> wReportedData;
map<string, double> wReportedTime;

TimeProfiler timeTotal;
TimeProfiler timeParse;
TimeProfiler timeTokenize;
TimeProfiler timeSort;
TimeProfiler timeFlush;

IndexWriter::IndexWriter(IndexConfig &config) {

	this->config = config;
	this->term_counter = 0;

	size_t num_blocks_max = config.getMemorySize() / config.getBlockSize();

	this->block_max_size = config.getBlockSize() / RECORD_SIZE;

	this->buffer_max_size = num_blocks_max * block_max_size;

	this->buffer.reserve(this->buffer_max_size);

	this->fileManager.open_file(config.getTmpFilePath(), MODE_WRITE);

	this->docFileManager = new DocFileManager(config.getDocFilePath(), "w");

	this->urlDictionary.reserve(config.getDocumentsMaxNumber());
}

IndexWriter::~IndexWriter() {

	wReportedData["number_of_terms"] = dictionary.size();
	wReportedData["memory_size"] = config.getMemorySize();

	for (auto it = wReportedTime.begin(); it != wReportedTime.end(); ++it) {
		printf("%s\t%g\n", it->first.c_str(), it->second);
	}

	for (auto it = wReportedData.begin(); it != wReportedData.end(); ++it) {
		printf("%s\t%ld\n", it->first.c_str(), it->second);
	}
}

void IndexWriter::writer(string &collection_directory,
		string &collection_index) {

	size_t document_counter = 0;

	timeTotal.start();

	// Open compressed collection collection
	RICPNS::CollectionReader reader(collection_directory, collection_index);
	RICPNS::Document document;
	HTMLParser parser;

	//***************************************************************************
	//	DocInfo docInfo;
	//	docInfo.setContent("To do is to be.\nTo be is to do.\n");
	//	docInfo.setUrl("doc1");
	//	this->addDocument(docInfo, 1);
	//
	//	docInfo.setContent("To be or not to be.\nI am what I am.\n");
	//	docInfo.setUrl("doc2");
	//	this->addDocument(docInfo, 2);
	//
	//	docInfo.setContent("I think therefore I am.\nDo be do be do.\n");
	//	docInfo.setUrl("doc3");
	//	this->addDocument(docInfo, 3);
	//
	//	docInfo.setContent("Do do do, da da da.\nLet it be, let it be.\n");
	//	docInfo.setUrl("doc4");
	//	this->addDocument(docInfo, 4);
	//***************************************************************************



	// Open temporary links file
	string linkFileName = config.getDirectory() + "linksTemp";
	this->fpLinks = fopen(linkFileName.c_str(), "w+");

	// Parse each document of the collection
	while (reader.getNextDocument(document)) {

		// Increments document counter
		document_counter++;

		// Parse document
		timeParse.start();
		DocInfo docInfo = parser.parse(document);
		wReportedTime["parse_time"] += timeParse.reportTime();

		// Index document
		timeTokenize.start();
		this->addDocument(docInfo, document_counter);
		wReportedTime["tokenize_time"] += timeTokenize.reportTime();

		// Checks if reach documents limit
		if (document_counter >= config.getDocumentsMaxNumber()) {
			break;
		}

		document.clear();
	}

	// Creates the anchor file
	processAnchors(document_counter);

	// Flush the remaining records stored in buffer to disk
	if (buffer.size() > 0) {
		flushBuffer();
	}

	// Save the dictionary
	DictFileManager dictFileManager;
	dictFileManager.save_dictionary(config.getDictFilePath(), dictionary);
	this->fileManager.close_file();
	docFileManager->saveIndex(config.getDocIndexFilePath());

	// Close links file
	fclose(fpLinks);
	//	remove(linkFileName.c_str());

	wReportedTime["total_time"] = timeTotal.reportTime();
	wReportedData["number_of_docs"] = document_counter;
}

void IndexWriter::addDocument(DocInfo &doc, size_t doc_counter) {

	if(doc_counter % 10000 == 0){
		cout << doc_counter << endl;
	}

	//Adds the document to the url hash map
	//	urlDictionary[doc.getUrl()] = doc_counter;
	urlDictionary[md5(doc.getCanonicalUrl())] = doc_counter;

	// Tokenize the document to get its terms
	unordered_map<unsigned int, unsigned int> termFrequency;
	countTermFrequency(termFrequency, doc.getContent() + " " + doc.getTitle());

	// For each term of the document, write it on the buffer
	for (auto it = termFrequency.begin(); it != termFrequency.end(); ++it) {

		Record record(it->first, doc_counter, it->second);
		buffer.push_back(record);

		// If the run is full, write, sort and write it on disk
		if (buffer.size() == buffer_max_size) {
			flushBuffer();
		}
	}

	// Store document metadata (title and url)
	docFileManager->saveDocument(doc_counter, doc.getTitle(), doc.getUrl());

	// Save links in tmp file
	saveLinks(doc_counter, doc.getLinks());
}

void IndexWriter::countTermFrequency(
		unordered_map<unsigned int, unsigned int> & termFrequency,
		string content) {

	// Tokenize the content into terms
	Tokenizer tokenizer(content);

	while (tokenizer.hasNext()) {
		string term = tokenizer.getNext();
		if (term.length() > 50) {
			continue;
		}
		if (!dictionary[term]) {
			dictionary[term] = ++term_counter;
		}
		termFrequency[dictionary[term]]++;
	}
}

void IndexWriter::flushBuffer() {

	// Sort
	timeSort.start();

	Quicksort quicksort;
	quicksort.sort(buffer);

	wReportedTime["sort_time"] += timeSort.reportTime();

	// Save
	timeFlush.start();

	int num_blocks = 1 + (buffer.size() - 1) / block_max_size;
	this->fileManager.write(buffer, block_max_size * num_blocks);

	wReportedTime["flush_time"] += timeFlush.reportTime();

	// Clear
	buffer.clear();
	wReportedData["number_of_runs"]++;
}

void IndexWriter::processAnchors(size_t &document_counter) {

	printf("Processing anchors\n");

	unordered_map<unsigned int, list<unsigned int> > ingoingLinks;
	unordered_map<unsigned int, unsigned int> outgoingLinks;

	char termStr[51], urlStr[33];
	size_t docNumber, numLinks;
	unsigned int sourceDocId;

	rewind(fpLinks);

	while (!feof(fpLinks)) {

		fscanf(fpLinks, "%u %lu\n", &sourceDocId, &numLinks);

		size_t links_matched = 0;
		wReportedData["number_of_links_total"] += numLinks;

		for (size_t i = 0; i < numLinks; i++) {

			fscanf(fpLinks, "%33s\n", urlStr);
			string url = string(urlStr);

			unordered_map<unsigned int, unsigned int> termFrequency;

			unordered_map<string, unsigned int>::const_iterator url_iter =
					urlDictionary.find(url);

			// Check if the link points to some document in the collection
			// if not, add the link to the collection
			if (url_iter != urlDictionary.end() && url_iter->second != sourceDocId) {
				docNumber = url_iter->second;
				links_matched++;
			} else {
//				docNumber = ++document_counter;
//				urlDictionary[url] = docNumber;
//				outgoingLinks[docNumber] = 0;

				while (fscanf(fpLinks, "%50s", termStr) > 0) {
					if (strcmp(termStr, ">#") == 0)
						break;
				}
				continue;
			}

			// Add link to the graph
			ingoingLinks[docNumber].push_back(sourceDocId);

			// Read the terms, and add them to a buffer as a triple
			while (fscanf(fpLinks, "%50s", termStr) > 0) {
				string term = string(termStr);
				if (term == ">#")
					break;

				if (!dictionary[term]) {
					dictionary[term] = ++term_counter;
				}
				termFrequency[dictionary[term]]++;
			}

			// For each term of the document, write it on the buffer
			for (auto it = termFrequency.begin(); it != termFrequency.end();
					++it) {
				//				cout << it->first << " " << docNumber << " "  << it->second << endl;
				Record record(it->first, docNumber, it->second);
				buffer.push_back(record);

				if (buffer.size() == buffer_max_size) {
					flushBuffer();
				}
			}
		}

		outgoingLinks[sourceDocId] = links_matched;
		wReportedData["number_of_links_matched"] += links_matched;
	}

	// Save ingoingLinks graph
	saveInLinks(ingoingLinks);
	saveOutLinks(outgoingLinks);

}

void IndexWriter::removeDuplicates(vector<Record> &records) {
	size_t j = 0;
	Record baseRecord = records[0];
	for (Record record : records) {
		if (baseRecord.getTerm() == record.getTerm()
				&& baseRecord.getDocument() == record.getDocument()) {
			baseRecord.setFrequency(
					baseRecord.getFrequency() + record.getFrequency());
		} else {
			records[j++] = baseRecord;
			baseRecord = record;
		}
	}
	records[j] = baseRecord;
	records.resize(j + 1);
}

void IndexWriter::saveLinks(unsigned int docId,
		list<pair<string, string> > links) {

	fprintf(fpLinks, "%u %lu\n", docId, links.size());

	for (pair<string, string> link : links) {

		if(link.first.length() > wReportedData["max_url_size"]){
			wReportedData["max_url_size"] = link.first.length();
		}

		//		fprintf(fpLinks, "%s\n", link.first.c_str());
		fprintf(fpLinks, "%s\n", md5(link.first).c_str());

		Tokenizer tokenizer(link.second);

		while (tokenizer.hasNext()) {
			string term = tokenizer.getNext();
			if (term.length() > 50) {
				continue;
			}
			fprintf(fpLinks, "%s ", term.c_str());
		}
		fprintf(fpLinks, ">#\n");
	}
}

void IndexWriter::saveInLinks(
		unordered_map<unsigned int, list<unsigned int> > &inLinks) {

	// TODO: set graph name in config
	string graphName = config.getDirectory() + "inLinks";
	FILE *fp = fopen(graphName.c_str(), "wb"); // TODO: use binary file

	for (auto it = inLinks.begin(); it != inLinks.end(); ++it) {

		unsigned int v = it->first;
		unsigned int numEdges = it->second.size();

		fwrite(&v, sizeof(unsigned int), 1, fp);
		fwrite(&numEdges, sizeof(unsigned int), 1, fp);

//		fprintf(fp, "%u %lu\n", it->first, it->second.size());

		for (unsigned int item : it->second) {
//						fprintf(fp, "%u\n", item);
			fwrite(&item, sizeof(unsigned int), 1, fp);
		}
	}

	fclose(fp);
}

void IndexWriter::saveOutLinks(
		unordered_map<unsigned int, unsigned int> &outLinks) {

	// TODO: set graph name in config
	string graphName = config.getDirectory() + "outLinks";
	FILE *fp = fopen(graphName.c_str(), "wb");

	for (auto it = outLinks.begin(); it != outLinks.end(); ++it) {

//				fprintf(fp, "%u %u\n", it->first, it->second);
		fwrite(&it->first, sizeof(unsigned int), 1, fp);
		fwrite(&it->second, sizeof(unsigned int), 1, fp);
	}

	fclose(fp);
}
