/*
 * HTMLParser.cpp
 *
 *  Created on: Feb 22, 2014
 *      Author: lizardo
 */

#include "HTMLParser.h"

#include <assert.h>
#include <stack>
#include <iostream>
#include <regex>
//#include <thread>

#include "oneurl/Url.h"

void HTMLParser::extractContent(GumboNode *node, string &content) {

	content = "";

	stack<GumboNode*> mystack;
	mystack.push(node);

	while (!mystack.empty()) {
		GumboNode* u = mystack.top();
		mystack.pop();

		if (u->type == GUMBO_NODE_TEXT) {
			content.append(" " + string(u->v.text.text));
		} else if (u->type == GUMBO_NODE_ELEMENT
				&& u->v.element.tag != GUMBO_TAG_SCRIPT
				&& u->v.element.tag != GUMBO_TAG_STYLE) {

			GumboVector* children = &u->v.element.children;

			for (unsigned i = 0; i < children->length; ++i) {
				mystack.push((GumboNode*) children->data[i]);
			}
		}
	}
}

void HTMLParser::extractLinks(GumboNode *node,
		list<pair<string, string> > &links, string &docURL) {

	if (node->type != GUMBO_NODE_ELEMENT) {
		return;
	}

	GumboAttribute* href;
	if (node->v.element.tag == GUMBO_TAG_A
			&& (href = gumbo_get_attribute(&node->v.element.attributes, "href"))) {

		string anchor;

		if (node->v.element.children.length > 0) {
			GumboNode* title_text =
					(GumboNode*) node->v.element.children.data[0];
			anchor = title_text->v.text.text;
		} else {
			anchor = "";
		}

		string url = treatLink(docURL, href->value);
		if (!url.empty()) {
			links.push_back(make_pair(url, anchor));
		}
	}

	GumboVector* children = &node->v.element.children;
	for (size_t i = 0; i < children->length; ++i) {
		extractLinks(static_cast<GumboNode*>(children->data[i]), links, docURL);
	}
}

void HTMLParser::extractPageTitle(GumboNode *node, string &title) {

	//	assert(node->type == GUMBO_NODE_ELEMENT);
	//	assert(node->v.element.children.length >= 2);

	const GumboVector* root_children = &node->v.element.children;
	GumboNode* head = NULL;
	for (unsigned int i = 0; i < root_children->length; ++i) {
		GumboNode* child = (GumboNode*) root_children->data[i];
		if (child->type == GUMBO_NODE_ELEMENT
				&& child->v.element.tag == GUMBO_TAG_HEAD) {
			head = child;
			break;
		}
	}
	//	assert(head != NULL);

	GumboVector* head_children = &head->v.element.children;
	for (unsigned int i = 0; i < head_children->length; ++i) {
		GumboNode* child = (GumboNode*) head_children->data[i];
		if (child->type == GUMBO_NODE_ELEMENT
				&& child->v.element.tag == GUMBO_TAG_TITLE) {
			if (child->v.element.children.length != 1) {
				title = "";
				return;
			}
			GumboNode* title_text =
					(GumboNode*) child->v.element.children.data[0];
			//			assert(title_text->type == GUMBO_NODE_TEXT);
			title = title_text->v.text.text;
			return;
		}
	}
	title = "";
}

void HTMLParser::cleanText(string text, string &cleanedText) {
	size_t initP = text.find("<html");

	if (initP < text.npos)
		cleanedText = text.substr(initP, text.npos);
	else {
		initP = text.find("<body");
		if (initP < text.npos)
			cleanedText = text.substr(initP, text.npos);
		else {
			initP = text.find("<div");
			if (initP < text.npos)
				cleanedText = text.substr(initP, text.npos);
			else
				cleanedText = text;
		}
	}
}

string HTMLParser::treatLink(string docURL, string link) {
	oneurl curl;

	string fixed = curl.CNormalize(link);
	if (!fixed.empty()) {
//		cout << "OK: " << link << endl;
		return fixed;
	}
	else if (regex_match (link, regex("(javascript:|JavaScript:|mailto:|MailTo:|file:|File:|ftp:|Ftp:).*|(.*@.*)"))) {
//		cout << "Remove: " << link << endl;
		return "";
	} else if (link[0] == '.') {
		curl.CNormalize(docURL);
		fixed = curl.GetScheme() + "://" + curl.GetHost()
				+ curl.GetPath() + link;
//		cout << "Fix.: " << link << " -  " << curl.CNormalize(fixed) << endl;
		return curl.CNormalize(fixed);
	} else if (link[0] == '/') {
		curl.CNormalize(docURL);
		fixed = curl.GetScheme() + "://" + curl.GetHost() + link;
//		cout << "Fix/: " << link << " -  " << curl.CNormalize(fixed) << endl;
		return curl.CNormalize(fixed);
	} else if (link[0] != '#') {
		curl.CNormalize(docURL);
		fixed = curl.GetScheme() + "://" + curl.GetHost()
				+ curl.GetPath().substr(0, curl.GetPath().find_last_of('/') + 1)
				+ link;
//		cout << "Fixa: " << link << " -  " << curl.CNormalize(fixed) << endl;
		return curl.CNormalize(fixed);
	} else {
//		cout << "Error: " << link << endl;
		return "";
	}
}

void HTMLParser::bar(string *x){
	cout << *x << endl;
}

DocInfo HTMLParser::parse(RICPNS::Document &document) {

	oneurl curl;
	string html;

	cleanText(document.getText(), html);
	GumboOutput* output = gumbo_parse(html.c_str());
	GumboNode* node = output->root;

	string docUrl = document.getURL();
	string content, pageTitle;
	list<pair<string, string> > links;

//	thread t1(&HTMLParser::extractContent, this, node, ref(content));
//	thread t2(&HTMLParser::extractPageTitle, this, node, ref(pageTitle));
//	thread t3(&HTMLParser::extractLinks, this, node, ref(links), ref(docUrl));

	extractContent(node, content);
	extractPageTitle(node, pageTitle);
	extractLinks(node, links, docUrl);

//	t1.join();
//	t2.join();
//	t3.join();

	gumbo_destroy_output(&kGumboDefaultOptions, output);

	DocInfo docInfo;
	docInfo.setContent(content);

	docInfo.setCanonicalUrl(
			curl.Parse(docUrl) ?
					curl.CNormalize(docUrl) : docUrl);

	docInfo.setUrl(docUrl);

	docInfo.setTitle(pageTitle);
	docInfo.setLinks(links);
	//	cout << docInfo.getUrl() << "  -  "<<  "   " << link << endl;

//	static int i=1;
//	cout << i++ << " - " << docInfo.getCanonicalUrl() << endl;
//
//
//	for(pair<string, string> link : links){
//		cout <<  " ------- " << link.first << endl;
//		cout <<  link.second << endl;
//	}

	return docInfo;
}
