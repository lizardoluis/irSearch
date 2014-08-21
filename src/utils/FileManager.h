/*
 * FileManager.h
 *
 *  Created on: Mar 12, 2014
 *      Author: lizardo
 */

#ifndef FILEMANAGER_H_
#define FILEMANAGER_H_

#include "../commons/Record.h"

#include <list>
#include <vector>
#include <string>

#define MODE_WRITE "wb"
#define MODE_READ "rb"
#define MODE_READ_WRITE "rb+"

using namespace std;

/*
 * This class manages the file that contains the records. It offer read and
 * write functions and other file operation.
 */
class FileManager {
private:
	FILE *file;

	string filePath;

	void write_record(Record &record);

	size_t read_record(Record &record);

public:

	/*
	 * Default constructor.
	 */
	FileManager();

	/*
	 * Constructor.
	 *
	 * @param filePath: C string containing the name of the file to be opened.
	 * @param mode: C string containing a file access mode.
	 */
	FileManager(string filePath, const char * mode);

	/*
	 * Open the file.
	 *
	 * @param filePath: C string containing the name of the file to be opened.
	 * @param mode: C string containing a file access mode.
	 */
	void open_file(string filePath, const char * mode);

	/*
	 * Close the file.
	 */
	void close_file();

	/*
	 * Writer the record vector on a file.
	 *
	 * @param buffer: the vector containing the data.
	 * @buffer_max_size: size of data to be written. If it is bigger than
	 * 		the buffer size, padding will be added.
	 */
	void write(vector<Record> & buffer, size_t buffer_max_size);

	/*
	 * Writer the record vector on the file.
	 *
	 * @param buffer: the vector containing the data.
	 * @param buffer_max_size: size of data to be written. If it is bigger than
	 * 		the buffer size, padding will be added.
	 * @param pos: position in bytes in a file, where the data will be written.
	 */
	void write(vector<Record> & buffer, size_t buffer_max_size, size_t pos);

	/*
	 * Read the record vector from the file.
	 *
	 * @param buffer: the list to store the data.
	 * @param number_of_records: size of data to be read.
	 * @param pos: position in bytes in a file, where the data will be read.
	 */
	size_t read(list<Record> & buffer, size_t number_of_records, size_t pos);

	/*
	 * Read the record vector from the file.
	 *
	 * @param buffer: the vector to store the data.
	 * @param number_of_records: size of data to be read.
	 * @param pos: position in bytes in a file, where the data will be read.
	 */
	size_t read(vector<Record> & buffer, size_t number_of_records, size_t pos);

	/*
	 * Calculate the file size.
	 */
	size_t calc_file_size();

	/*
	 * Truncate the file to size in parameter.
	 */
	void fileTruncate(size_t size);

	//TODO: remove
	void print_file();
};

#endif /* FILEMANAGER_H_ */
