/*
 * BM25Query.cpp
 *
 *  Created on: Apr 16, 2014
 *      Author: lizardo
 */

#include "BM25Query.h"

#include "math.h"

#include <map>

ResultSet BM25Query::query(string query) {

	// Get query terms
	list<string> terms;
	parseQuery(query, terms);

	map <unsigned int, double> docsMap;


	//Constants
	double k1 = 1.0;
//	double b = 0.75;

	// For each term in the query
	for (string term : terms) {

		// Search term at vocabulary
		vector<term_info> inverted_list;
		index->searchTerm(term, inverted_list);

		unsigned int ni = inverted_list.size();

		// For each document in the inverted list of the term
		for(size_t i=0; i<inverted_list.size(); i++){
			unsigned int dj = inverted_list[i].doc;
			unsigned int fij = inverted_list[i].freq;

			double Bij = ((k1 + 1) * fij) /
					(k1 * ((1-b) + b * ((double)index->getDocumentLen(dj)/index->getAvgDocLen())) + fij);

			docsMap[dj] += Bij * log2((index->getNumberOfDocuments() - ni + 0.5)/(ni + 0.5));
		}
	}

	// Flip map
	multimap<double, unsigned int> flippedDocMap;
	for (map<unsigned int, double>::iterator it=docsMap.begin(); it!=docsMap.end(); ++it){
		flippedDocMap.insert ( make_pair(it->second, it->first) );
	}

	list<unsigned int> docIds;
	for (multimap<double, unsigned int>::reverse_iterator it=flippedDocMap.rbegin(); it!=flippedDocMap.rend(); ++it){
		unsigned int d = it->second;
		docIds.push_back(d);
	}

	ResultSet result(docFileManager, docIds);

	return result;
}
