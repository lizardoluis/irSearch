/*
 * ResultSet.h
 *
 *  Created on: Apr 8, 2014
 *      Author: lizardo
 */

#ifndef RESULTSET_H_
#define RESULTSET_H_

#include <list>
#include <string>

#include "../utils/DocFileManager.h"

using namespace std;

//typedef struct {
//	string title;
//	string url;
//} doc_result;

class ResultSet {
private:
	size_t size;

	list<unsigned int> docIds;

	DocFileManager *docFileManager;

public:

	ResultSet(){}

	ResultSet(DocFileManager *docFileManager, list<unsigned int> docIds);

	bool hasNext();

	DocInfo next();

	size_t getSize();
};

#endif /* RESULTSET_H_ */
