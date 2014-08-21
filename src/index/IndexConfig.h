/*
 * IndexConfig.h
 *
 *  Created on: Mar 11, 2014
 *      Author: lizardo
 */

#ifndef INDEXCONFIG_H_
#define INDEXCONFIG_H_

#include <string>

using namespace std;

/*
 * This class sets up the main information needed to run the algorithms. It
 * should be passed the IndexWriter and IndexSorter classes.
 */
class IndexConfig {
private:

	/*
	 * Directory where the files will be written during execution.
	 */
	string directory;

	/*
	 * Temporary file path.
	 */
	string tmp_file_path;

	/*
	 * Temporary anchors file path.
	 */
	string tmp_anchor_file_path;

	/*
	 * Dictionary file path.
	 */
	string dict_file_path;

	/*
	 * Dictionary anchors file path.
	 */
	string dict_anchor_file_path;

	/*
	 * Dictionary file path.
	 */
	string index_file_path;

	/*
	 * Document info file path.
	 */
	string doc_file_path;

	/*
	 * Document info file path.
	 */
	string docIndex_file_path;

	/*
	 * Memory size in MB.
	 */
	size_t memory_size;

	/*
	 * Block size in KB.
	 */
	size_t block_size;

	/*
	 * Max number of documents to process.
	 */
	size_t documents_max_number;

public:

	IndexConfig() {
	}

	IndexConfig(size_t memory_size, size_t documents_max_number);

	size_t getDocumentsMaxNumber() const;

	void setDocumentsMaxNumber(size_t documentsMaxNumber);

	size_t getMemorySize() const;

	/*
	 * Set the memory size in MB.
	 */
	void setMemorySize(size_t memorySize);

	const string& getDirectory() const;

	void setDirectory(const string& Directory);

	const string& getTmpFilePath() const;

	void setTmpFileName(const string& tmpFilePath);

	const string& getTmpAnchorFilePath() const;

	void setTmpAnchorFileName(const string& tmpFilePath);

	const string& getDictFilePath() const;

	void setDictFileName(const string& tmpFilePath);

	const string& getDictAnchorFilePath() const;

	void setDictAnchorFileName(const string& tmpFilePath);

	const string& getIndexFilePath() const;

	void setIndexFileName(const string& indexFilePath);

	const string& getDocFilePath() const;

	void setDocFileName(const string& docFilePath);

	const string& getDocIndexFilePath() const;

	void setDocIndexFileName(const string& docIndexFilePath);

	size_t getBlockSize() const;

	/*
	 * Set the block size in KB.
	 */
	void setBlockSize(size_t blockSize);
};

#endif /* INDEXCONFIG_H_ */
