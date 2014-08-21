/*
 * Lexicon.h
 *
 *  Created on: Apr 1, 2014
 *      Author: lizardo
 */

#ifndef LEXICON_H_
#define LEXICON_H_

#include <vector>
#include <stdio.h>

using namespace std;

typedef struct {
	char *term;
	unsigned int size;
	size_t pos;
	double idf;
} lexicon_t;

class Lexicon {
private:

	size_t lex_size;

	vector<lexicon_t> lex;

public:

	Lexicon(size_t size);

	void add(size_t p, char *term, unsigned int size, size_t pos, double idf);

	char* getTerm(size_t p);

	unsigned int getSize(size_t p);

	size_t getPos(size_t p);

	size_t getLexSize() const;

	double getIdf(size_t p);

	void print(){

		for(size_t i=0; i<lex.size(); i+=2){
			printf("%s %lu %u %g\n", lex[i].term, lex[i].pos, lex[i].size, lex[i].idf);
		}
		printf("\n");
	}
};

#endif /* LEXICON_H_ */
