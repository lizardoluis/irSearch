/*
 * DocInfo.cpp
 *
 *  Created on: Feb 22, 2014
 *      Author: lizardo
 */

#include "DocInfo.h"

DocInfo::DocInfo() {
}

const string& DocInfo::getContent() const {
	return content;
}

void DocInfo::setContent(const string& content) {
	this->content = content;
}

const list< pair<string, string> >& DocInfo::getLinks() const {
	return links;
}

void DocInfo::addLink(const pair<string, string>& link) {
	this->links.push_back(link);
}

void DocInfo::setLinks(const list< pair<string, string> >& links) {
	this->links = links;
}

const string& DocInfo::getTitle() const {
	return title;
}

void DocInfo::setTitle(const string& title) {
	this->title = title;
}

const string& DocInfo::getUrl() const {
	return url;
}

const string& DocInfo::getCanonicalUrl() const {
	return canonicalUrl;
}

void DocInfo::setCanonicalUrl(const string& canonicalUrl) {
	this->canonicalUrl = canonicalUrl;
}

void DocInfo::setUrl(const string& url) {
	this->url = url;
}
