/*
 * IndexSorter.h
 *
 *  Created on: Mar 13, 2014
 *      Author: lizardo
 */

#ifndef INDEXSORTER_H_
#define INDEXSORTER_H_

#include "../commons/Record.h"
#include "../index/IndexConfig.h"
#include "../utils/FileManager.h"

#include <string>
#include <vector>

using namespace std;

/*
 * This class implements the in-place multiway merging algorithm. For proper
 * operation, the memory size set up in the configuration must be the same used
 * during the writer.
 */
class IndexSorter {
private:

	/*
	 * Contains some configuration stuff.
	 */
	IndexConfig config;

	/*
	 * This object is responsible for managing the file.
	 */
	FileManager fileManager;

	/*
	 * This function uses the free list to find a vacant block, and write the
	 * block in the disk
	 */
	void flushOutputBlock(vector<Record> &outputBlock, list<size_t> &freeBlocks,
			vector<size_t> &blockTable, size_t block_size);

	/*
	 * Read the next block front its run
	 */
	void readNextBlock(list<Record> &inputBlock, list<size_t> &freeBlocks,
			size_t &pos, size_t &inputBlocksRemaining, size_t block_size);

	/*
	 * In-place permutation algorithm.
	 */
	void inPlacePermutation(vector<size_t> &blockTable, size_t block_size);

public:

	/*
	 * Constructor. The memory size set up inside the config, must be the same
	 * used during the writer operation.
	 *
	 * @param config: memory size, output folder, and temporary partially
	 * sorted file name must be set up in this object.
	 */
	IndexSorter(IndexConfig &config);

	/*
	 * Destrutor. It prints the time statistics collected during execcution.
	 */
	~IndexSorter();

	/*
	 * This function sorts a file with the sort-based inversion algorithm
	 */
	void sort();
};

#endif /* INDEXSORTER_H_ */
