/*
 * VectorQueryPageRank.h
 *
 *  Created on: Apr 11, 2014
 *      Author: lizardo
 */

#ifndef VECTORQUERYPAGERANK_H_
#define VECTORQUERYPAGERANK_H_

#include "Query.h"
#include "PageRank.h"

#define NORM_VECTOR 1
#define NORM_WORDS 2
#define NORM_NO 3

using namespace std;

class VectorQueryPageRank: public Query {
private:

	PageRank * pageRank;

	int norm;

	double alfa;

public:
	VectorQueryPageRank(Index &index, DocFileManager &docFileManager, int norm, PageRank &pageRank, double alfa) :
			Query(index, docFileManager) {
		this->norm = norm;
		this->pageRank = &pageRank;
		this->alfa = alfa;
	};

	~VectorQueryPageRank(){}

	ResultSet query(string query);
};

#endif /* VECTORQUERYPAGERANK_H_ */
