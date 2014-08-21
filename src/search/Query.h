/*
 * Query.h
 *
 *  Created on: Apr 17, 2014
 *      Author: lizardo
 */

#ifndef QUERY_H_
#define QUERY_H_

#include <list>
#include <string>

#include "PageRank.h"
#include "ResultSet.h"
#include "../utils/DocFileManager.h"
#include "../index/Index.h"

using namespace std;

class Query {
protected:

	Index *index;

	DocFileManager *docFileManager;

	void parseQuery(string& query, list<string>& terms);

public:
	Query(Index &index, DocFileManager &docFileManager);

	virtual ~Query();

	/*
	 * Execute the query. The results are return in the ResultSet.
	 */
	virtual ResultSet query(string query) = 0;
};

#endif /* QUERY_H_ */
