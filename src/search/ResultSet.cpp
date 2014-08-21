/*
 * ResultSet.cpp
 *
 *  Created on: Apr 8, 2014
 *      Author: lizardo
 */

#include "ResultSet.h"

ResultSet::ResultSet(DocFileManager *docFileManager,
		list<unsigned int> docIds) {
	this->docIds = docIds;

	this->docFileManager = docFileManager;

	this->size = docIds.size();

	/*while(!docIds.empty()){
		printf("%u\n", docIds.front());
		docIds.pop_front();
	}*/
}

bool ResultSet::hasNext() {
	return !docIds.empty();
}

DocInfo ResultSet::next() {

	unsigned int id = docIds.front();

	docIds.pop_front();

	DocInfo result = docFileManager->loadDocument(id);

	return result;
}

size_t ResultSet::getSize() {
	return size;
}
