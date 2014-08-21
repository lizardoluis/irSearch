/*
 * Query.cpp
 *
 *  Created on: Apr 17, 2014
 *      Author: lizardo
 */

#include "Query.h"
#include "../analysis/Tokenizer.h"

Query::Query(Index& index, DocFileManager& docFileManager) {

	this->index = &index;
	this->docFileManager = &docFileManager;
}

Query::~Query() {

	// TODO Auto-generated destructor stub
}

void Query::parseQuery(string& query, list<string>& terms) {
	terms.clear();

	Tokenizer tokenizer(query);
	for (int i = 0; tokenizer.hasNext(); i++) {
		string token = tokenizer.getNext();
		bool add = true;

		for (list<string>::iterator it=terms.begin(); it != terms.end(); ++it){
			if(*it == token){
				add = false;
				break;
			}
		}

		if(add){
			terms.push_back(token);
		}
	}
}
