/*
 * Tokenizer.h
 *
 *  Created on: Feb 24, 2014
 *      Author: lizardo
 */

#ifndef TOKENIZER_H_
#define TOKENIZER_H_

#include <string>
#include <locale>

using namespace std;

class Tokenizer {
private:
	locale loc;
	wstring wtext;
	unsigned currentIndex;

	bool isException(wchar_t c);
	char removeAccent(wchar_t c);
	string narrow(wstring const& s);
	wstring widen(string const& s);

public:
	Tokenizer(string &text);
	string getNext();
	bool hasNext();
};

#endif /* TOKENIZER_H_ */
