//============================================================================
// Name        : index_writer_test.cpp
// Author      : Luis Lizardo
// Version     :
// Copyright   :
// Description : This program process text and write the temporary file.
//============================================================================

#include <string>

#include <cstdio>
#include <cstdlib>

#include "index/IndexConfig.h"
#include "index/IndexWriter.h"
#include "index/IndexSorter.h"

using namespace std;

#define MAX_MEM 1024 //1GB
#define MAX_DOCS 99999999

/*
 * Shows how to use this program.
 */
void show_usage() {
	printf("Usage: index_writer_sorter -i <collection directory> "
			"-c <collection index> -d <output directory> "
			"-m <memory size (MB)> "
			"-n <max document number> \n");
}

/*
 * Gets command line arguments
 */
bool parse_arguments(int argc, char** argv, string &collection_directory,
		string &collection_index, string &directory, size_t &memory_size,
		size_t &documents_number) {

	bool j = false, k = false, l = false;

	// Parse the arguments from command line
	for (int i = 1; i < argc; i++) {
		string param(argv[i]);
		if (param == "--collectionDirectory" || param == "-i") {
			collection_directory = string(argv[++i]);
			j = true;
		} else if (param == "--collectionIndex" || param == "-c") {
			collection_index = string(argv[++i]);
			k = true;
		} else if (param == "--directory" || param == "-d") {
			directory = string(argv[++i]);
			l = true;
		} else if (param == "--memorySize" || param == "-m") {
			if (sscanf(argv[++i], "%ld", &memory_size) == EOF)
				return false;
		} else if (param == "--documentsNumber" || param == "-n") {
			if (sscanf(argv[++i], "%ld", &documents_number) == EOF)
				return false;
		}
	}

	if (j && k && l)
		return true;
	else
		return false;
}

int main(int argc, char** argv) {

	string directory;
	string collection_index;
	string output_directory;

	size_t memory_size = MAX_MEM;
	size_t max_documents = MAX_DOCS;

	// Parse the command line arguments
	if (!parse_arguments(argc, argv, directory, collection_index,
			output_directory, memory_size, max_documents)) {
		show_usage();
		exit(EXIT_FAILURE);
	}

	// Set up index configurations
	IndexConfig config(memory_size, max_documents);
	config.setDirectory(output_directory);
	config.setTmpFileName("triples");
	config.setTmpAnchorFileName("triples-anchors");
	config.setDictFileName("dict");
	config.setDictAnchorFileName("dict-anchors");
	config.setBlockSize(50);
	config.setDocFileName("docInfo");
	config.setDocIndexFileName("docInfoIndex");

	// Create temporary file
	printf("Parsing collection...\n");

	IndexWriter index_writer(config);
	index_writer.writer(directory, collection_index);

	printf("Sorting temporary file...\n");

	IndexSorter index_sorter(config);
	index_sorter.sort();

	return 0;
}
