/*
 * PageRank.h
 *
 *  Created on: May 9, 2014
 *      Author: lizardo
 */

#ifndef PAGERANK_H_
#define PAGERANK_H_

#include <list>
#include <unordered_map>
#include <vector>
#include <string>

using namespace std;

#define EPSILON (0.000000001)
#define DUMPING_FACTOR (0.15)
#define MAX_ITERATIONS (150)

class PageRank {
private:

	double dumping_factor;

	vector<double> rank;

	void calcRank(unordered_map<unsigned int, unsigned int> &numOutgoingLinks,
			unordered_map<unsigned int, list<unsigned int> > &ingoingLinks,
			unsigned int numDocs);

	void loadOutLinks(string filePath,
			unordered_map<unsigned int, unsigned int> &outLinks);

	void loadInLinks(string filePath,
			unordered_map<unsigned int, list<unsigned int> > &inLinks);

public:
	PageRank(string outgoingLinksFilePath, string ingoingLinksFilePath,
			unsigned int numDocs, double dumping_factor);

	PageRank(string outgoingLinksFilePath, string ingoingLinksFilePath,
				unsigned int numDocs){
		PageRank(outgoingLinksFilePath, ingoingLinksFilePath, numDocs, DUMPING_FACTOR);
	}

	double getRank(unsigned int docId);
};

#endif /* PAGERANK_H_ */
