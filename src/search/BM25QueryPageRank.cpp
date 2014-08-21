/*
 * BM25QueryPageRank.cpp
 *
 *  Created on: Apr 16, 2014
 *      Author: lizardo
 */

#include "BM25QueryPageRank.h"

#include "math.h"

#include <map>

struct classcomp {
	bool operator()(const pair<unsigned int, double>& a,
			const pair<unsigned int, double>& b) const {
		return a.second > b.second;
	}
};

ResultSet BM25QueryPageRank::query(string query) {

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

	// Calculates maximum and minumum values for normalization
	double max=0.0, min=1000.0;
	for(auto i = docsMap.begin(); i != docsMap.end(); ++i){
		if(i->second > max) max = i->second;
		if(i->second < min) min = i->second;
	}

	// Flip map
	multimap<double, unsigned int> flippedDocMap;
	for(auto it = docsMap.begin(); it != docsMap.end(); ++it){
		double v = (it->second-min)/(max-min);
		double res = alfa*v + (1-alfa)*pageRank->getRank(it->first);
		flippedDocMap.insert ( make_pair(res, it->first) );
	}

	list<unsigned int> docIds;
	for (multimap<double, unsigned int>::reverse_iterator it=flippedDocMap.rbegin(); it!=flippedDocMap.rend(); ++it){
		unsigned int d = it->second;
		docIds.push_back(d);
	}

	ResultSet result(docFileManager, docIds);

	return result;
}
