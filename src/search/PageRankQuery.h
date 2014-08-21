/*
 * PageRankQuery.h
 *
 *  Created on: Jun 1, 2014
 *      Author: lizardo
 */

#ifndef PAGERANKQUERY_H_
#define PAGERANKQUERY_H_

#include "PageRank.h"
#include "Query.h"
#include "ResultSet.h"
#include "../index/Index.h"
#include <string>

class PageRankQuery: public Query {
private:

	PageRank *pageRank;

public:

	PageRankQuery(Index &index, DocFileManager &docFileManager, PageRank &pageRank) :
			Query(index, docFileManager) {
		this->pageRank = &pageRank;
	};

	void top();

	ResultSet query(string query);
};

#endif /* PAGERANKQUERY_H_ */
