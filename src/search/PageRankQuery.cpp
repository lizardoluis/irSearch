/*
 * PageRankQuery.cpp
 *
 *  Created on: Jun 1, 2014
 *      Author: lizardo
 */

#include "PageRankQuery.h"

#include <algorithm>
#include <iostream>
#include <set>

struct classcomp {
	bool operator()(const pair<unsigned int, double>& a,
			const pair<unsigned int, double>& b) const {
		return a.second > b.second;
	}
};

//ResultSet PageRankQuery::query(string query) {
//
//	// Get query terms
//	list<string> terms;
//	parseQuery(query, terms);
//	set< pair<unsigned int, double>, classcomp > docsSet;
//
//	// For each term in the query
//	for (string term : terms) {
//
//		// Search term at vocabulary
//		vector<term_info> inverted_list;
//		index->searchTerm(term, inverted_list);
//
//		// Flip map
//		for (term_info doc : inverted_list){
//			docsSet.insert(make_pair(doc.doc, pageRank->getRank(doc.doc)));
//		}
//	}
//
//	list<unsigned int> docIds;
//	for (pair<unsigned int, double> item : docsSet){
//		docIds.push_back(item.first);
//	}
//
//	ResultSet result(docFileManager, docIds);
//
//	return result;
//}

void PageRankQuery::top(){

	set< pair<unsigned int, double>, classcomp > docsSet;

	for(size_t i=1; i<=945642; i++){

		docsSet.insert(make_pair(i, pageRank->getRank(i)));

	}

	int i=0;
	for (pair<unsigned int, double> item : docsSet){
		DocInfo docInfo = docFileManager->loadDocument(item.first);
		cout << item.first << " - " << docInfo.getUrl() << " - " << docInfo.getCanonicalUrl() << " - " << item.second << endl;
		if(i++ == 10) break;
	}
}

ResultSet PageRankQuery::query(string query) {

	// Get query terms
	list<string> terms;
	parseQuery(query, terms);
	set< pair<unsigned int, double>, classcomp > docsSet;

	//	map<unsigned int, unsigned int> docsMap;

	// For each term in the query
	for (string term : terms) {

		cout << term << endl;

		// Search term at vocabulary
		vector<term_info> inverted_list;
		index->searchTerm(term, inverted_list);

		// Flip map
		for (term_info doc : inverted_list){
			//			docsMap[doc.doc]++;
			//			if(docsMap[doc.doc] == terms.size()){
			//				docsSet.insert(make_pair(doc.doc, pageRank->getRank(doc.doc)));
			//			}
			docsSet.insert(make_pair(doc.doc, pageRank->getRank(doc.doc)));
		}
	}

	list<unsigned int> docIds;
	for (pair<unsigned int, double> item : docsSet){
		docIds.push_back(item.first);
	}

	ResultSet result(docFileManager, docIds);

	return result;
}
