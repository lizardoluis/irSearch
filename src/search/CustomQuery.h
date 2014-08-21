/*
 * CustomQuery.h
 *
 *  Created on: Apr 11, 2014
 *      Author: lizardo
 */

#ifndef CUSTOMQUERY_H_
#define CUSTOMQUERY_H_

#include "Query.h"

using namespace std;

class CustomQuery: public Query {
public:
	CustomQuery(Index &index, DocFileManager &docFileManager) :
			Query(index, docFileManager) {
	};

	ResultSet query(string query);
};

#endif /* CUSTOMQUERY_H_ */
