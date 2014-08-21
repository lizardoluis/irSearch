/*
 * IndexConfig.cpp
 *
 *  Created on: Mar 11, 2014
 *      Author: lizardo
 */

#include "IndexConfig.h"

IndexConfig::IndexConfig(size_t memory_size, size_t documents_max_number) {
	this->setMemorySize(memory_size);
	this->setDocumentsMaxNumber(documents_max_number);
	this->setBlockSize(50);
}

size_t IndexConfig::getDocumentsMaxNumber() const {
	return documents_max_number;
}

void IndexConfig::setDocumentsMaxNumber(size_t documentsMaxNumber) {
	documents_max_number = documentsMaxNumber;
}

size_t IndexConfig::getMemorySize() const {
	return memory_size;
//	return 10000;
}

void IndexConfig::setMemorySize(size_t memorySize) {
	if(memorySize < 1)
		memorySize = 1;
	memory_size = memorySize*1048576;
}

const string& IndexConfig::getDirectory() const {
	return directory;
}

void IndexConfig::setDirectory(const string& directory) {

	if(directory.empty()){
		this->directory = "";
	}
	else if(directory[directory.size()-1] != '/'){
		this->directory = directory + '/';
	}
	else
		this->directory = directory;
}

const string& IndexConfig::getTmpFilePath() const {
	return tmp_file_path;
}

void IndexConfig::setTmpFileName(const string& tmpFilePath) {
	tmp_file_path = directory + tmpFilePath;
}

const string& IndexConfig::getTmpAnchorFilePath() const {
	return tmp_anchor_file_path;
}

void IndexConfig::setTmpAnchorFileName(const string& tmpFilePath) {
	tmp_anchor_file_path = directory + tmpFilePath;
}

const string& IndexConfig::getDictFilePath() const {
	return dict_file_path;
}

void IndexConfig::setDictFileName(const string& tmpFilePath) {
	dict_file_path = directory + tmpFilePath;
}

const string& IndexConfig::getDictAnchorFilePath() const {
	return dict_anchor_file_path;
}

void IndexConfig::setDictAnchorFileName(const string& tmpFilePath) {
	dict_anchor_file_path = directory + tmpFilePath;
}

size_t IndexConfig::getBlockSize() const {
	return block_size;
}

void IndexConfig::setBlockSize(size_t blockSize) {
	if(blockSize < 1)
		blockSize = 1;

	if(blockSize*1024 > memory_size)
		blockSize = memory_size/4;

	block_size = blockSize*1024;
}

const string& IndexConfig::getIndexFilePath() const {
	return index_file_path;
}

void IndexConfig::setIndexFileName(const string& indexFilePath) {
	index_file_path = directory + indexFilePath;
}

const string& IndexConfig::getDocFilePath() const {
	return doc_file_path;
}

void IndexConfig::setDocFileName(const string& docFilePath) {
	doc_file_path = directory + docFilePath;
}

const string& IndexConfig::getDocIndexFilePath() const {
	return docIndex_file_path;
}

void IndexConfig::setDocIndexFileName(const string& docIndexFilePath) {
	docIndex_file_path = directory + docIndexFilePath;
}
