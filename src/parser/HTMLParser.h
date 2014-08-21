/*
 * HTMLParser.h
 *
 *  Created on: Feb 22, 2014
 *      Author: lizardo
 */

#ifndef HTMLPARSER_H_
#define HTMLPARSER_H_

#include "gumbo.h"
#include "../commons/DocInfo.h"

#include <Document.h>
#include <list>

using namespace std;

/*
 * This class parses an HTML document extracting its content, links  and title.
 */
class HTMLParser {
private:

	/*
	 * Parses the html document and extracts the content without tags,
	 * links, scripts or style css.
	 */
	void extractContent(GumboNode *node, string &content);

	/*
	 * Parses the html document and extracts links to other pages.
	 */
	void extractLinks(GumboNode *node, list< pair<string, string> > &links, string &docURL);

	/*
	 * Parses the html document and extracts the document title.
	 */
	void extractPageTitle(GumboNode *node, string &title);

	/*
	 * Cleans the document text extracting non HTML text out of it.
	 */
	void cleanText(string text, string &cleanedText);

	/*
	 * Heuristic to treat and normalize links.
	 */
	string treatLink(string docURL, string link);

	void bar(string *x);

public:

	DocInfo parse(RICPNS::Document &document);
};

#endif /* HTMLPARSER_H_ */
