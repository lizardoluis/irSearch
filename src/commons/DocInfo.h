/*
 * DocInfo.h
 *
 *  Created on: Feb 22, 2014
 *      Author: lizardo
 */

#ifndef HTMLDOC_H_
#define HTMLDOC_H_

#include <list>
#include <string>

using namespace std;

/*
 * This class represents a HTML document after being cleaned by the parser.
 * Here the content is cleared of HTML tags.
 */
class DocInfo {
private:

	/*
	 * URL of the HTML document.
	 */
	string url;

	string canonicalUrl;

	/*
	 * Title of the HTML document.
	 */
	string title;

	/*
	 * Text of the HTML document without tags.
	 */
	string content;

	/*
	 * Links inside the document other pages.
	 */
	list< pair<string, string> > links;

public:
	/*
	 * Constructor.
	 */
	DocInfo();

	/*
	 * Setters and getters.
	 */
	const string& getContent() const;

	void setContent(const string& content);

	const list< pair<string, string> >& getLinks() const;

	void setLinks(const list< pair<string, string> >& links);

	void addLink(const pair<string, string>& link);

	const string& getTitle() const;

	void setTitle(const string& title);

	const string& getUrl() const;

	void setUrl(const string& url);
	const string& getCanonicalUrl() const;
	void setCanonicalUrl(const string& canonicalUrl);
};

#endif /* HTMLDOC_H_ */
