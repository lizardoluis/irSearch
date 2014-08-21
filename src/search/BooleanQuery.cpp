/*
 * BooleanQuery.cpp
 *
 *  Created on: Apr 3, 2014
 *      Author: lizardo
 */

#include "BooleanQuery.h"
#include "../analysis/Tokenizer.h"

#define AND 0
#define OR 1

ResultSet BooleanQuery::query(string query) {

	list<string> terms;
	list<boolean_op> ops;

	vector<term_info> result_list;

	// Parse and tokenize query string
	parseQuery(query, terms, ops);

	index->searchTerm(terms.front(), result_list);
	terms.pop_front();

	for (list<string>::iterator it=terms.begin(); it != terms.end(); ++it){
		vector<term_info> a_list;
		index->searchTerm(*it, a_list);

		if(!ops.empty()){
			if(ops.front() == AND){
				andOp(result_list, a_list);
			}
			else {
				orOp(result_list, a_list);
			}
			ops.pop_front();
		}
		else{
			printf("Query error!\n");
			break;
		}
	}

	list<unsigned int> docIds;

	for(size_t i=0; i<result_list.size(); i++){
		docIds.push_back(result_list[i].doc);
	}

	ResultSet result(docFileManager, docIds);

	return result;
}

void BooleanQuery::parseQuery(string &query, list<string> &terms, list<boolean_op> &ops) {

	terms.clear();
	ops.clear();

	Tokenizer tokenizer(query);
	for (int i = 0; tokenizer.hasNext(); i++) {
		string token = tokenizer.getNext();

		if (i % 2 == 0) {
			terms.push_back(token);
		} else if (token == "and") {
			ops.push_back(AND);
		} else if (token == "or") {
			ops.push_back(OR);
		} else {
			ops.push_back(AND);
			terms.push_back(token);
			i--;
		}
	}

}

void BooleanQuery::andOp(vector< term_info > & list1, vector< term_info > & list2){

	vector< term_info > inverted_list;

	size_t size1 = list1.size();
	size_t size2 = list2.size();

	size_t i=0, j=0;

	while(i < size1 && j < size2){

		if( list1[i].doc < list2[j].doc ){
			i++;
		}
		else if( list2[j].doc < list1[i].doc ){
			j++;
		}
		else if( list2[j].doc == list1[i].doc ){
			term_info info;
			info.doc = list1[i].doc;
			inverted_list.push_back( info );
			i++; j++;
		}
	}

	list1 = inverted_list;
}

void BooleanQuery::orOp(vector< term_info > & list1, vector< term_info > & list2){

	vector< term_info > inverted_list;

	size_t size1 = list1.size();
	size_t size2 = list2.size();

	size_t i=0, j=0;

	while(i < size1 && j < size2){

		if( list1[i].doc < list2[j].doc ){
			inverted_list.push_back( list1[i] );
			i++;
		}
		else if( list2[j].doc < list1[i].doc ){
			inverted_list.push_back( list2[j] );
			j++;
		}
		else if( list2[j].doc == list1[i].doc ){
			term_info info;
			info.doc = list1[i].doc;
			inverted_list.push_back( info );
			i++; j++;
		}
	}

	while(i < size1){
		inverted_list.push_back( list1[i] );
		i++;
	}

	while(j < size2){
		inverted_list.push_back( list2[j] );
		j++;
	}

	list1 = inverted_list;
}
