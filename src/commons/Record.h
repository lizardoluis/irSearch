/*
 * Record.h
 *
 *  Created on: Mar 3, 2014
 *      Author: lizardo
 */

#ifndef RECORD_H_
#define RECORD_H_

#define RECORD_SIZE 12

//TODO: remove this library
#include <iostream>


class Record {
private:
	unsigned int term;
	unsigned int document;
	unsigned int frequency;

public:
	/*
	 * Constructors.
	 */
	Record();

	Record(unsigned int term, unsigned int document, unsigned int frequency);

	/*
	 * Setters and getters.
	 */
	unsigned int getDocument() const;

	void setDocument(unsigned int document);

	unsigned int getFrequency() const;

	void setFrequency(unsigned int frequency);

	unsigned int getTerm() const;

	void setTerm(unsigned int term);

	// TODO: remove this function
	void print();
};

#endif /* RECORD_H_ */
