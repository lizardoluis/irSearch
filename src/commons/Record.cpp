/*
 * Record.cpp
 *
 *  Created on: Mar 3, 2014
 *      Author: lizardo
 */

#include "Record.h"


Record::Record(){
	this->term = 0;
	this->document = 0;
	this->frequency = 0;
}

Record::Record(unsigned int term, unsigned int document, unsigned int frequency) {
	this->term = term;
	this->document = document;
	this->frequency = frequency;
}

void Record::print(){
	std::cout << term << " " << document << " " << frequency << std::endl;
}

unsigned int Record::getDocument() const {
	return document;
}

void Record::setDocument(unsigned int document) {
	this->document = document;
}

unsigned int Record::getFrequency() const {
	return frequency;
}

void Record::setFrequency(unsigned int frequency) {
	this->frequency = frequency;
}

unsigned int Record::getTerm() const {
	return term;
}

void Record::setTerm(unsigned int term) {
	this->term = term;
}
