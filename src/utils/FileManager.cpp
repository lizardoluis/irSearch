/*
 * FileManager.cpp
 *
 *  Created on: Mar 12, 2014
 *      Author: lizardo
 */

#include "FileManager.h"

#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <climits>

FileManager::FileManager() {
	this->file = NULL;
}

FileManager::FileManager(string filePath, const char * mode) {
	this->file = NULL;
	this->filePath = filePath;

	this->open_file(filePath, mode);
}

void FileManager::write_record(Record& record) {
	unsigned int t = record.getTerm();
	unsigned int d = record.getDocument();
	unsigned int f = record.getFrequency();

	fwrite(&t, sizeof(unsigned int), 1, this->file);
	fwrite(&d, sizeof(unsigned int), 1, this->file);
	fwrite(&f, sizeof(unsigned int), 1, this->file);

	//	record.print();
}

size_t FileManager::read_record(Record& record) {
	unsigned int t, d, f;
	size_t count = 0;

	count += fread(&t, sizeof(unsigned int), 1, this->file);
	count += fread(&d, sizeof(unsigned int), 1, this->file);
	count += fread(&f, sizeof(unsigned int), 1, this->file);

	record.setTerm(t);
	record.setDocument(d);
	record.setFrequency(f);

	return count;
}

void FileManager::open_file(string filePath, const char * mode) {
	this->filePath = filePath;
	this->file = fopen(filePath.c_str(), mode);
	if (this->file == NULL) {
		printf("Error when opening file.");
		exit(0);
	}
}

void FileManager::close_file() {

	if (this->file != NULL) {
		fclose(this->file);
	}
}

void FileManager::write(vector<Record>& buffer, size_t buffer_max_size) {

	for (size_t i = 0; i < buffer.size(); ++i) {
		this->write_record(buffer[i]);
	}

	// Add padding if the buffer size is lower than the buffer_max_size
	size_t padding_size = buffer_max_size - buffer.size();
	if (padding_size) {
		Record r(UINT_MAX, UINT_MAX, UINT_MAX);
		for (size_t i = 0; i < padding_size; i++) {
			this->write_record(r);
		}
	}
}

void FileManager::write(vector<Record> & buffer, size_t buffer_max_size,
		size_t pos) {
	size_t err = fseek(file, pos, SEEK_SET);
	if (err != 0) {
		printf("Error setting the block location.");
		exit(0);
	}

	this->write(buffer, buffer_max_size);
}

size_t FileManager::read(list<Record>& buffer, size_t number_of_records,
		size_t pos) {

	size_t err = fseek(file, pos, SEEK_SET);
	if (err != 0) {
		printf("Error setting the block location.");
		exit(0);
	}

	buffer.clear();
	Record record;
	for (size_t i = 0; i < number_of_records; i++) {
		if (read_record(record) == 3)
			buffer.push_back(record);
		else
			break;
	}

	return ftell(file);
}

size_t FileManager::read(vector<Record>& buffer, size_t number_of_records,
		size_t pos) {

	size_t err = fseek(file, pos, SEEK_SET);
	if (err != 0) {
		printf("Error setting the block location.");
		exit(0);
	}

	buffer.clear();
	Record record;
	for (size_t i = 0; i < number_of_records; i++) {
		if (read_record(record) == 3)
			buffer.push_back(record);
		else
			break;
	}

	return ftell(file);
}

void FileManager::print_file() {

	rewind(file);

	Record r;
	while (read_record(r) == 3) {
		r.print();
	}
}

size_t FileManager::calc_file_size() {
	fseek(file, 0L, SEEK_END);
	size_t size = ftell(file);
	rewind(file);
	return size;
}

void FileManager::fileTruncate(size_t size) {
	if (truncate64(filePath.c_str(), size) != 0)
		perror("ftruncate() error");
}
