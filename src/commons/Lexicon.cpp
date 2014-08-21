/*
 * Lexicon.cpp
 *
 *  Created on: Apr 1, 2014
 *      Author: lizardo
 */

#include "Lexicon.h"

#include <cstring>

Lexicon::Lexicon(size_t size) {
	lex_size = 0;
	lex.resize(size);
}

void Lexicon::add(size_t p, char* term, unsigned int size, size_t pos, double idf) {
	lex[p].term = term;
	lex[p].size = size;
	lex[p].pos = pos;
	lex[p].idf = idf;

	// Only for statistics
	lex_size += strlen(term) + sizeof(long unsigned int) + sizeof(unsigned int);
}

char* Lexicon::getTerm(size_t p) {
	return lex[p].term;
}

unsigned int Lexicon::getSize(size_t p) {
	return lex[p].size;
}

size_t Lexicon::getPos(size_t p) {
	return lex[p].pos;
}

double Lexicon::getIdf(size_t p) {
	return lex[p].idf;
}

size_t Lexicon::getLexSize() const {
	return lex_size;
}


