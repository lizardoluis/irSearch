/*
 * Tokenizer.cpp
 *
 *  Created on: Feb 24, 2014
 *      Author: lizardo
 */

#include "Tokenizer.h"

#include <iostream>
#include <vector>
#include <cassert>
#include <iomanip>
#include <cctype>
#include <cwctype>
#include <map>

Tokenizer::Tokenizer(string &text) {
	loc = locale("en_US.UTF8");
	wtext = widen(text);
	currentIndex = 0;
}

string Tokenizer::getNext() {

	wstring wstr;
	string str;

	for (; currentIndex < wtext.length(); currentIndex++) {
		wchar_t c = wtext[currentIndex];

		c = removeAccent(c);
		c = towlower(c);

		if (iswalpha(c) || iswdigit(c) || isException(c)) {
			wstr.push_back(c);
			if (currentIndex < wtext.length() - 1) {
				continue;
			}
		}

		if (wstr.length() > 0) {
			str = narrow(wstr);
			currentIndex++;
			break;
		}
	}

	return str;
}

bool Tokenizer::hasNext() {
	for (; currentIndex < wtext.length(); currentIndex++) {
		wchar_t c = wtext[currentIndex];
		c = removeAccent(c);
		if (iswalpha(c) || iswdigit(c) || isException(c)) {
			return true;
		}
	}
	return false;
}

bool Tokenizer::isException(wchar_t c) {
	switch (c) {
	case L'%':
//	case L'\'':
	case L'$':
		return true;
	default:
		return false;
	}
}

char Tokenizer::removeAccent(wchar_t c) {
	switch (c) {
	case L'à':
	case L'á':
	case L'â':
	case L'ã':
	case L'ä':
	case L'å':
	case L'À':
	case L'Á':
	case L'Â':
	case L'Ã':
	case L'Ä':
	case L'Å':
		return 'a';
	case L'ç':
	case L'Ç':
	case L'¢':
		return 'c';
	case L'è':
	case L'é':
	case L'ê':
	case L'ë':
	case L'È':
	case L'É':
	case L'Ê':
	case L'Ë':
		return 'e';
	case L'ì':
	case L'í':
	case L'î':
	case L'ï':
	case L'Ì':
	case L'Í':
	case L'Î':
	case L'Ï':
		return 'i';
	case L'ñ':
	case L'Ñ':
		return 'n';
	case L'ò':
	case L'ó':
	case L'ô':
	case L'õ':
	case L'ö':
	case L'Ò':
	case L'Ó':
	case L'Ô':
	case L'Õ':
	case L'Ö':
	case L'Ø':
		return 'o';
	case L'ù':
	case L'ú':
	case L'û':
	case L'ü':
	case L'Ù':
	case L'Ú':
	case L'Û':
	case L'Ü':
	case L'µ':
		return 'u';
	case L'Š':
		return 's';
	case L'ý':
	case L'ÿ':
	case L'Ý':
	case L'Ÿ':
		return 'y';
	case L'Þ':
		return 'p';
	default:
		return c;
	}
}

string Tokenizer::narrow(wstring const& s) {
	//@see: http://goo.gl/tgZwMl

	vector<char> result(4 * s.size() + 1);
	wchar_t const* fromNext;
	char* toNext;
	mbstate_t state = { 0 };
//	codecvt_base::result convResult = use_facet<
//			codecvt<wchar_t, char, mbstate_t> >(loc).out(state, &s[0],
//			&s[s.size()], fromNext, &result[0], &result[result.size()], toNext);

	use_facet<codecvt<wchar_t, char, mbstate_t> >(loc).out(state, &s[0],
			&s[s.size()], fromNext, &result[0], &result[result.size()], toNext);

	//TODO remove asserts and adds conditionals
//	assert(fromNext == &s[s.size()]);
//	assert(toNext != &result[result.size()]);
//	assert(convResult == codecvt_base::ok);
	*toNext = '\0';

	return &result[0];
}

wstring Tokenizer::widen(string const& s) {
	//@see: http://goo.gl/tgZwMl
//	cout << s << endl;

	vector<wchar_t> result(s.size() + 1);
	char const* fromNext;
	wchar_t* toNext;
	mbstate_t state = { 0 };
//	codecvt_base::result convResult = use_facet<
//			codecvt<wchar_t, char, mbstate_t> >(loc).in(state, &s[0],
//					&s[s.size()], fromNext, &result[0], &result[result.size()], toNext);

	use_facet<codecvt<wchar_t, char, mbstate_t> >(loc).in(state, &s[0],
			&s[s.size()], fromNext, &result[0], &result[result.size()], toNext);

	//TODO remove asserts and adds conditionals
//	assert(fromNext == &s[s.size()]);
//	assert(toNext != &result[result.size()]);
//	assert(convResult == codecvt_base::ok);
	*toNext = L'\0';

	return &result[0];
}
