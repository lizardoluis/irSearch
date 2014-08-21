/*
 * BM25Query.h
 *
 *  Created on: Apr 16, 2014
 *      Author: lizardo
 */

#ifndef BM25QUERY_H_
#define BM25QUERY_H_

#include "Query.h"

using namespace std;

class BM25Query: public Query {
private:

	double b;

public:
	BM25Query(Index &index, DocFileManager &docFileManager, double b) :
			Query(index, docFileManager) {
		this->b = b;
	};

	ResultSet query(string query);
};

#endif /* BM25QUERY_H_ */
