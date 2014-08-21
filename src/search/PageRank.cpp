/*
 * PageRank.cpp
 *
 *  Created on: May 9, 2014
 *      Author: lizardo
 */

#include "PageRank.h"
#include <list>
#include <cmath>
#include <iostream>
#include "../utils/TimeProfiler.h"

#include <algorithm>

PageRank::PageRank(string outgoingLinksFilePath, string ingoingLinksFilePath,
		unsigned int numDocs, double dumping_factor){

	unordered_map<unsigned int, unsigned int> numOutgoingLinks;
	unordered_map<unsigned int, list<unsigned int> > ingoingLinks;

	//Open and read ingoing links
	loadOutLinks(outgoingLinksFilePath, numOutgoingLinks);

	//Open and read outgoing links
	loadInLinks(ingoingLinksFilePath, ingoingLinks);


	rank.resize(numDocs, 1.0/numDocs);

	this->dumping_factor = dumping_factor;

	calcRank(numOutgoingLinks, ingoingLinks, numDocs);
}

void PageRank::calcRank(
		unordered_map<unsigned int, unsigned int>& numOutgoingLinks,
		unordered_map<unsigned int, list<unsigned int> >& ingoingLinks,
		unsigned int numDocs) {

	TimeProfiler timeProfiler;

	timeProfiler.start();

	size_t j;
	for(j=0; j<MAX_ITERATIONS; j++){

		double currentDelta = 0.0;

		for(size_t d=1; d<=numDocs; d++){

			double sum = 0;

			for(unsigned int link : ingoingLinks[d]){
				sum += rank[link-1]/numOutgoingLinks[link];
			}

			double r = dumping_factor/numDocs + (1.0 - dumping_factor)*sum;

			currentDelta = max(currentDelta, abs(rank[d-1] - r));

			rank[d-1] = r;
		}

		if(currentDelta < EPSILON){
			break;
		}
	}
}

double PageRank::getRank(unsigned int docId) {
	return rank[docId-1];
}

void PageRank::loadOutLinks(string filePath, unordered_map<unsigned int, unsigned int> &outLinks) {

	FILE *fp = fopen(filePath.c_str(), "rb");

	unsigned int u, v;

	while(!feof(fp)){
		//		fprintf(fp, "%u %u\n", it->first, it->second);
		fread(&u, sizeof(unsigned int), 1, fp);
		fread(&v, sizeof(unsigned int), 1, fp);

		outLinks[u] = v;
	}

	fclose(fp);
}

void PageRank::loadInLinks(string filePath,
		unordered_map<unsigned int, list<unsigned int> > &inLinks) {

	FILE *fp = fopen(filePath.c_str(), "rb");

	unsigned int v, numEdges, edge;

	while(!feof(fp)){

		fread(&v, sizeof(unsigned int), 1, fp);
		fread(&numEdges, sizeof(unsigned int), 1, fp);

		list<unsigned int> edges;

		for (unsigned int i=0; i< numEdges; i++) {
			fread(&edge, sizeof(unsigned int), 1, fp);
			edges.push_back(edge);
		}

		inLinks[v] = edges;
	}

	fclose(fp);
}
