/*
 * Index.h
 *
 *  Created on: Apr 1, 2014
 *      Author: lizardo
 */

#ifndef INDEX_H_
#define INDEX_H_

#include "../commons/Lexicon.h"
#include "../index/hash.h"
#include "../index/IndexConfig.h"

#include <map>
#include <unordered_map>
#include <string>

using namespace std;

typedef struct {
	unsigned int doc;
	unsigned int freq;
} term_info;

class Index {
private:

	Lexicon *lexicon;

	Hash *hash;

	FILE *indexFile;

	unsigned int number_of_documents;

	double avg_doclen;

	unordered_map<unsigned int, double> documentNorm;

	unordered_map<unsigned int, unsigned int> documentLen;

	void parseTriplesFile(char** dict, string filePath);

	void getInvertedList(size_t pos, size_t size,
			vector<term_info>& inverted_list);

public:

	Index(IndexConfig &config);

	~Index();

	/*
	 * Search for a term in the index and returns its inverted list.
	 */
	unsigned int searchTerm(string term, vector<term_info> &inverted_list);

	/*
	 * return Number of documentos int the collection
	 */
	unsigned int getNumberOfDocuments();

	/*
	 * return IDF do termo.
	 */
	double getTermIDF(string term);

	/*
	 * return Norma vetorial do documento.
	 */
	double getDocumentNorm(unsigned int doc);

	/*
	 * return Norma do documento em numero de termos.
	 */
	unsigned int getDocumentLen(unsigned int doc);

	/*
	 * return Tamanho medio dos documentos da colecao.
	 */
	double getAvgDocLen();
};

#endif /* INDEX_H_ */
