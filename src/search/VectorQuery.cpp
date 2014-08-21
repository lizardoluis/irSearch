/*
 * VectorQuery.cpp
 *
 *  Created on: Apr 11, 2014
 *      Author: lizardo
 */

#include "VectorQuery.h"
#include "math.h"
#include "../analysis/Tokenizer.h"

#include <map>

ResultSet VectorQuery::query(string query) {

	// Get query terms
	list<string> terms;
	parseQuery(query, terms);

	map <unsigned int, double> docsMap;

	// For each term in the query
	for (list<string>::iterator it=terms.begin(); it != terms.end(); ++it) {

		// Get term
		string term = *it;

		// Search term at vocabulary
		vector<term_info> inverted_list;
		index->searchTerm(term, inverted_list);

		// Calculate idf
		double idf = index->getTermIDF(term);

		// For each document in the inverted list of the term
		for(size_t i=0; i<inverted_list.size(); i++){
			unsigned int doc = inverted_list[i].doc;
			unsigned int freq = inverted_list[i].freq;
			docsMap[doc] += (1 + log2(freq)) * idf;
		}
	}

	// Divide by the document norm
	multimap<double, unsigned int> flippedDocMap;
	for (map<unsigned int, double>::iterator it=docsMap.begin(); it!=docsMap.end(); ++it){
		unsigned int d = it->first;
		if(norm == NORM_VECTOR)
			flippedDocMap.insert ( make_pair(it->second/index->getDocumentNorm(d), d) );
		else if(norm == NORM_WORDS)
			flippedDocMap.insert ( make_pair(it->second/index->getDocumentLen(d), d) );
		else
			flippedDocMap.insert ( make_pair(it->second, d) );
	}

	list<unsigned int> docIds;
	for (multimap<double, unsigned int>::reverse_iterator it=flippedDocMap.rbegin(); it!=flippedDocMap.rend(); ++it){
		unsigned int d = it->second;
		docIds.push_back(d);
	}

	ResultSet result(docFileManager, docIds);

	return result;
}
