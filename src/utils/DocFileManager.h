/*
 * DocFileManager.h
 *
 *  Created on: Apr 8, 2014
 *      Author: lizardo
 */

#ifndef DOCFILEMANAGER_H_
#define DOCFILEMANAGER_H_

#include <map>
#include <string>
#include <cstdio>

#include "../commons/DocInfo.h"

using namespace std;

class DocFileManager {
private:
	//maps document id and position on the file
	map <size_t, size_t> docIndex;

	FILE *docFile;

public:
	DocFileManager(string docFilePath, string mode);

	~DocFileManager();

	void saveDocument(size_t id, string title, string url);

	DocInfo loadDocument(size_t id);

	void saveIndex(string indexFilePath);

	void loadIndex(string indexFilePath);
};

#endif /* DOCFILEMANAGER_H_ */
