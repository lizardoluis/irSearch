/*
 * BooleanQuery.h
 *
 *  Created on: Apr 3, 2014
 *      Author: lizardo
 */

#ifndef BOOLEANQUERY_H_
#define BOOLEANQUERY_H_

#include "Query.h"

typedef short boolean_op;

using namespace std;

class BooleanQuery : public Query {
private:

	void parseQuery(string &query, list<string> &terms, list<boolean_op> &ops);

	void andOp(vector< term_info > & list1, vector< term_info > & list2);

	void orOp(vector< term_info > & list1, vector< term_info > & list2);

	void notOp(vector< term_info > & list1, vector< term_info > & list2);

public:
	BooleanQuery(Index &index, DocFileManager &docFileManager) : Query(index, docFileManager) {};

	ResultSet query(string query);
};

#endif /* BOOLEANQUERY_H_ */
