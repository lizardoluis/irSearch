/*
 * BM25QueryPageRank.h
 *
 *  Created on: Apr 16, 2014
 *      Author: lizardo
 */

#ifndef BM25QUERYPAGERANK_H_
#define BM25QUERYPAGERANK_H_

#include "Query.h"
#include "PageRank.h"

#include <iostream>

using namespace std;

class BM25QueryPageRank: public Query {
private:

	double b;

	double alfa;

	PageRank *pageRank;

public:
	BM25QueryPageRank(Index &index, DocFileManager &docFileManager, double b, PageRank &pageRank, double alfa) :
			Query(index, docFileManager) {
		this->b = b;
		this->pageRank = &pageRank;
		this->alfa = alfa;
	};

	ResultSet query(string query);
};

#endif /* BM25QUERY_H_ */
