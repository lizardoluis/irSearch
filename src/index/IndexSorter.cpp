/*
 * IndexSorter.cpp
 *
 *  Created on: Mar 13, 2014
 *      Author: lizardo
 */

#include "IndexSorter.h"

#include <cstdio>
#include <map>
#include <list>
#include <climits>

#include "../sort/Heap.h"
#include "../utils/TimeProfiler.h"

#define SIZE_T_MAX numeric_limits<size_t>::max()

map<string, size_t> sReportedData;
map<string, double> sReportedTime;

TimeProfiler timeSortTotal;
TimeProfiler timePermutation;
TimeProfiler timeRead;
TimeProfiler timeWrite;

IndexSorter::IndexSorter(IndexConfig& config) {

	timeSortTotal.start();

	this->config = config;

	this->fileManager.open_file(config.getTmpFilePath(), MODE_READ_WRITE);
}

IndexSorter::~IndexSorter() {

	sReportedTime["total_sort_time"] = timeSortTotal.reportTime();

	for (map<string, double>::iterator it = sReportedTime.begin();
			it != sReportedTime.end(); ++it) {
		printf("%s\t%g\n", it->first.c_str(), it->second);
	}

	for (map<string, size_t>::iterator it = sReportedData.begin();
			it != sReportedData.end(); ++it) {
		printf("%s\t%ld\n", it->first.c_str(), it->second);
	}
}

void IndexSorter::sort() {

	size_t file_size = this->fileManager.calc_file_size();

	// Heap
	Heap heap;

	// Calculate block size in number of records
	size_t block_size = config.getBlockSize() / RECORD_SIZE;

	size_t blocks_per_run = config.getMemorySize() / config.getBlockSize();

	size_t run_size = blocks_per_run * (block_size);

	// Calculate number of runs
	size_t number_of_runs = 1 + ((file_size / RECORD_SIZE) - 1) / run_size;

	size_t number_of_blocks = 1 + ((file_size / RECORD_SIZE) -1) / block_size;

	// Heap output block
	vector<Record> outputBlock;

	// Heap input blocks
	vector<list<Record> > inputBlocks(number_of_runs);

	// Vector to store number of blocks per run remaining to be read
	vector<size_t> inputBlocksRemaining(number_of_runs, blocks_per_run);
	inputBlocksRemaining[number_of_runs - 1] = number_of_blocks
			- (number_of_runs - 1) * blocks_per_run;

	// Vector to store the next position to read file inside a run
	vector<size_t> inputBlocksPos(number_of_runs);

	// This list stores the position of free blocks in disk and their sizes, to
	// write back the output of the heap.
	list<size_t> freeBlocks;

	// This vector stores the position where the ordered output records are
	// written on disk, and will be used during the in place permutation.
	vector<size_t> blockTable(number_of_blocks);

	// To count the number of padding for truncate file in the future
	size_t padding_counter = 0;

	// Initialize input blocks, loading data into them and adding the first
	// record to the heap
	for (size_t i = 0; i < number_of_runs; i++) {

		inputBlocksPos[i] = i * run_size * RECORD_SIZE;

		readNextBlock(inputBlocks[i], freeBlocks, inputBlocksPos[i],
				inputBlocksRemaining[i], block_size);

		// Add the first record from each run into the heap
		Record record = inputBlocks[i].front();
		inputBlocks[i].pop_front();
		heap.push(record, i);
	}

	Record base_record, top_record;
	size_t top_block_number;
	bool watch_dog = true;
	Record padder(UINT_MAX, UINT_MAX, UINT_MAX);

	while (!heap.empty()) {

		//		Record top_record;
		//		size_t top_block_number;

		// Remove the root of the heap
		heap.top(top_record, top_block_number);

		// Happens only once
		if(watch_dog){
			base_record.setTerm(top_record.getTerm());
			base_record.setDocument(top_record.getDocument());
			base_record.setFrequency(0);
			watch_dog = false;
		}

		// Add it to the output block and check if there is duplicates due to the anchors
		if(top_record.getTerm() != UINT_MAX && top_record.getDocument() != UINT_MAX && base_record.getTerm() == top_record.getTerm() && base_record.getDocument() == top_record.getDocument()){
			base_record.setFrequency( base_record.getFrequency() + top_record.getFrequency() );
			heap.push(padder, UINT_MAX);
		}
		else{
			outputBlock.push_back(base_record);
			base_record = top_record;
		}

		// Count padding records
		if (top_record.getTerm() == UINT_MAX
				&& top_record.getDocument() == UINT_MAX
				&& top_record.getFrequency() == UINT_MAX) {
			padding_counter++;
		}

		// If the output block is full, write it in a vacant disk block.
		if (outputBlock.size() == block_size) {
			flushOutputBlock(outputBlock, freeBlocks, blockTable, block_size);
		}

		// If an input block becomes empty, read the next block from its run
		if(top_block_number != UINT_MAX){
			if (inputBlocks[top_block_number].empty()
					&& inputBlocksRemaining[top_block_number] > 0) {
				readNextBlock(inputBlocks[top_block_number], freeBlocks,
						inputBlocksPos[top_block_number],
						inputBlocksRemaining[top_block_number], block_size);
			}

			// Replace it by the next candidate from the same block
			if (!inputBlocks[top_block_number].empty()) {
				heap.push(inputBlocks[top_block_number].front(), top_block_number);
				inputBlocks[top_block_number].pop_front();
			}
		}
	}

	// Write the rest of the outputBlock
	if (outputBlock.size() > 0) {
		flushOutputBlock(outputBlock, freeBlocks, blockTable, block_size);
	}

	// Reorder the blocks so that the physical order corresponds to the logical
	// order
	cout << "Permutation" << endl;
	inPlacePermutation(blockTable, block_size);

	// Close file
	fileManager.close_file();

	// Truncate file to remove padding
	fileManager.fileTruncate(file_size - padding_counter * RECORD_SIZE);

	sReportedData["number_of_runs"] = number_of_runs;
	sReportedData["blocks_per_run"] = blocks_per_run;
}

void IndexSorter::readNextBlock(list<Record> &inputBlock,
		list<size_t> &freeBlocks, size_t &pos, size_t &inputBlocksRemaining,
		size_t block_size) {

	timeRead.start();
	size_t p = fileManager.read(inputBlock, block_size, pos);
	sReportedTime["read_time"] += timeRead.reportTime();
	sReportedData["number_of_reads"]++;

	freeBlocks.push_back(pos);
	pos = p;
	inputBlocksRemaining--;
}

void IndexSorter::flushOutputBlock(vector<Record> &outputBlock,
		list<size_t> &freeBlocks, vector<size_t> &blockTable,
		size_t block_size) {

	static size_t counter = 0;

	timeWrite.start();
	fileManager.write(outputBlock, block_size, freeBlocks.front());
	sReportedTime["write_time"] += timeWrite.reportTime();
	sReportedData["number_of_writes"]++;

	blockTable[freeBlocks.front() / (block_size * RECORD_SIZE)] = counter++;

	freeBlocks.pop_front();
	outputBlock.clear();
}

void IndexSorter::inPlacePermutation(vector<size_t> &blockTable,
		size_t block_size) {

	timePermutation.start();

	size_t block_size_bytes = block_size * RECORD_SIZE;
	size_t holding, vacant;

	vector<Record> memory, transfer;

	for (size_t i = 0; i < blockTable.size(); ++i) {

		if (i != blockTable[i]) {

			fileManager.read(memory, block_size, i * block_size_bytes);

			holding = blockTable[i];
			vacant = i;

			while (holding != vacant) {

				size_t j;
				for (j = 0; j < blockTable.size(); ++j) {
					if (blockTable[j] == vacant)
						break;
				}

				fileManager.read(transfer, block_size, j * block_size_bytes);
				fileManager.write(transfer, block_size,
						vacant * block_size_bytes);

				blockTable[vacant] = vacant;

				vacant = j;
			}

			fileManager.write(memory, block_size, vacant * block_size_bytes);

			blockTable[vacant] = holding;
		}
	}
	sReportedTime["permutation_time"] = timePermutation.reportTime();
}
