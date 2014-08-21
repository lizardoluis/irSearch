#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>

#include "index/IndexConfig.h"
#include "index/Index.h"
#include "search/ResultSet.h"
#include "search/BooleanQuery.h"
#include "search/VectorQuery.h"
#include "search/VectorQueryPageRank.h"
#include "search/BM25Query.h"
#include "search/CustomQuery.h"
#include "search/PageRankQuery.h"
#include "search/BM25QueryPageRank.h"
#include "utils/TimeProfiler.h"

#include "search/PageRank.h"

using namespace std;

/*
 * Shows how to use this program.
 */
void show_usage() {
	printf("Usage: index_query -d <directory> -t <temporary file name> "
			"-q <queries file path> -n <number of documents>\n");
}

/*
 * Gets command line arguments
 */
bool parse_arguments(int argc, char** argv, string &temporary_file_name,
		string &directory, string &query_file_path, size_t &documents_number) {

	bool t = false, q = false, d = false, n = false;

	// Parse the arguments from command line
	for (int i = 1; i < argc; i++) {
		string param(argv[i]);
		if (param == "--temporaryFileName" || param == "-t") {
			temporary_file_name = string(argv[++i]);
			t = true;
		} else if (param == "--directory" || param == "-d") {
			directory = string(argv[++i]);
			d = true;
		} else if (param == "--queryFilePath" || param == "-q") {
			query_file_path = string(argv[++i]);
			q = true;
		} else if (param == "--documentsNumber" || param == "-n") {
			n = true;
			if (sscanf(argv[++i], "%ld", &documents_number) == EOF)
				return false;
		}
	}

	if (t && d && q && n)
		return true;
	else
		return false;
}

void printResults(string directory, string model, string query,
		ResultSet &results) {

	string path = directory + "/" + model + "-" + query;

	FILE *fp = fopen(path.c_str(), "w");
	if (fp == NULL) {
		printf("Error openning the file.\n");
		exit(EXIT_FAILURE);
	}

	//			printf("Size: %lu\n", results.getSize());

	while (results.hasNext()) {

		DocInfo docInfo = results.next();

		//		printf("%s\n", docInfo.getUrl().c_str());
		fprintf(fp, "%s\n", docInfo.getUrl().c_str());
	}

	fclose(fp);
}

void executeQueries(string directory, string query_file_path, string model,
		Query &modelQuery) {

	char query[100];

	string path = directory + query_file_path;

	FILE *queryFP = fopen(path.c_str(), "r");
	if (queryFP == NULL) {
		printf("Error openning the file with queries.\n");
		exit(EXIT_FAILURE);
	}

	TimeProfiler timer;
	while (fscanf(queryFP, "%[^\n]\n", query) > 0) {
		timer.start();
		ResultSet results = modelQuery.query(query);
		printResults(directory, model, query, results);
	}

	fclose(queryFP);
}

int main(int argc, char **argv) {

	string directory;
	string temporary_file_name;
	string query_file_path;
	size_t documents_number;

	// Parse the command lineletricae arguments
	if (!parse_arguments(argc, argv, temporary_file_name, directory,
			query_file_path, documents_number)) {
		show_usage();
		exit(EXIT_FAILURE);
	}

	// Set up index configurations
	IndexConfig config;
	config.setDirectory(directory);
	config.setTmpFileName(temporary_file_name);
	config.setDictFileName("dict");
	config.setIndexFileName("index-compressed");
	config.setDocFileName("docInfo");
	config.setDocIndexFileName("docInfoIndex");
	config.setDocumentsMaxNumber(documents_number);

	//	PageRank
	string outPath = config.getDirectory() + "outLinks";
	string inPath = config.getDirectory() + "inLinks";

	PageRank pageRank(outPath, inPath, config.getDocumentsMaxNumber(), 0.45);

	// Build index
	Index index(config);

	DocFileManager docFileManager(config.getDocFilePath(), "r");
	docFileManager.loadIndex(config.getDocIndexFilePath());

	VectorQueryPageRank vectorQueryPageRank(index, docFileManager, NORM_VECTOR, pageRank, 0.1);
	executeQueries(config.getDirectory(), query_file_path, "cosine_pageRank", vectorQueryPageRank);

	BM25QueryPageRank bm25QueryPageRank(index, docFileManager, 0.0, pageRank, 0.1);
	executeQueries(config.getDirectory(), query_file_path, "bm25_pageRank", bm25QueryPageRank);

	PageRankQuery pageRankQuery(index, docFileManager, pageRank);
	executeQueries(config.getDirectory(), query_file_path, "pageRank", pageRankQuery);

	// Queries
	VectorQuery vectorQuery(index, docFileManager, NORM_NO);
	executeQueries(config.getDirectory(), query_file_path, "cosine", vectorQuery);

	BM25Query bm25Query(index, docFileManager, 0.0);
	executeQueries(config.getDirectory(), query_file_path, "bm25", bm25Query);

	CustomQuery customQuery(index, docFileManager);
	executeQueries(config.getDirectory(), query_file_path, "custom", customQuery);


	//
	//	BooleanQuery booleanQuery(index, docFileManager);
	//	executeQueries(config.getDirectory(), query_file_path, "boolean", booleanQuery);

	return 0;
}
