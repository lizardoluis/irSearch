/*
 * VectorQuery.h
 *
 *  Created on: Apr 11, 2014
 *      Author: lizardo
 */

#ifndef VECTORQUERY_H_
#define VECTORQUERY_H_

#include "Query.h"

#define NORM_VECTOR 1
#define NORM_WORDS 2
#define NORM_NO 3

using namespace std;

class VectorQuery: public Query {
private:

	int norm;

public:
	VectorQuery(Index &index, DocFileManager &docFileManager, int norm) :
			Query(index, docFileManager) {
		this->norm = norm;
	};

	~VectorQuery(){}

	ResultSet query(string query);
};

#endif /* VECTORQUERY_H_ */
