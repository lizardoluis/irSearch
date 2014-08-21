/*
 * DocFileManager.cpp
 *
 *  Created on: Apr 8, 2014
 *      Author: lizardo
 */

#include "DocFileManager.h"
#include "../analysis/utf8.h"

void replaceStrChar(string &str, string replace, char ch) {

	// set our locator equal to the first appearance of any character in replace
	size_t i = str.find_first_of(replace);

	while (i != string::npos) { // While our position in the sting is in range.
		str[i] = ch; // Change the character at position.
		i = str.find_first_of(replace, i + 1); // Relocate again.
	}
}

void fix_utf8_string(string& str)
{
    std::string temp;
    try{
    utf8::replace_invalid(str.begin(), str.end(), back_inserter(temp));
    str = temp;
    }
    catch(exception& e){
    	printf("%s\n", str.c_str());
    }
}

DocFileManager::DocFileManager(string docFilePath, string mode) {
	docFile = fopen(docFilePath.c_str(), mode.c_str());
}

DocFileManager::~DocFileManager() {
	fclose(docFile);
}

void DocFileManager::saveDocument(size_t id, string title, string url) {

	replaceStrChar(title, "\n", ' ');

	size_t pos = ftell(docFile);

	docIndex[id] = pos;

	fix_utf8_string(title);
//	url = urlEncode(url);

	if(title.empty() || title == " ") title = "-";
	if(url.empty() || url == " ") url = "-";

	fprintf(docFile, "%s\n%s\n", title.c_str(), url.c_str());
//	fprintf(docFile, "%lu %s\n", id, url.c_str());
}

void DocFileManager::saveIndex(string indexFilePath) {

	FILE *indexFile = fopen(indexFilePath.c_str(), "w");

	for (map<size_t, size_t>::iterator it=docIndex.begin(); it!=docIndex.end(); ++it){
		fprintf(indexFile, "%lu\n", it->second);
	}

	fclose(indexFile);
}

void DocFileManager::loadIndex(string indexFilePath) {

	size_t i=1, pos;
	FILE *indexFile = fopen(indexFilePath.c_str(), "r");
	docIndex.clear();

	while ( fscanf(indexFile, "%lu\n", &pos) > 0){
		docIndex[i++] = pos;
	}

	fclose(indexFile);
}

DocInfo DocFileManager::loadDocument(size_t id) {

	DocInfo doc;

	size_t pos = docIndex[id];

	fseek(docFile, pos, SEEK_SET);

	char title[2000], url[1000];

	fscanf(docFile, "%2000[^\n]\n%1000[^\n]\n", title, url);
//	fscanf(docFile, "%lu %s\n", &loadedId, url);

	string strTitle = string(title);
	size_t i=0;
	while(strTitle[i] == ' ' || strTitle[i] == '\t'){
		i++;
	}
	strTitle.erase(strTitle.begin(), strTitle.begin() + i);
	fix_utf8_string(strTitle);
	replaceStrChar(strTitle, "\t", ' ');

	doc.setTitle(strTitle);
	doc.setUrl(string(url));

	return doc;
}
