#include <stdio.h>
#include <string>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "oneurl/Url.h"

using namespace std;

void loadLines(string path, vector <string> &lines){
	FILE *fp = fopen(path.c_str(), "r");
	if(fp == NULL){
		printf("Error openning the file.\n");
		exit(EXIT_FAILURE);
	}

	char line[1000];
	while(fscanf(fp, "%[^\n]\n", line) > 0){
		lines.push_back(string(line));
	}

	fclose(fp);
}

bool searchRelevant(string url, vector <string> &relevantList){

	for (vector <string>::iterator it=relevantList.begin(); it!=relevantList.end(); ++it){
		if(url == (*it)){
			relevantList.erase(it);
			return true;
		}
	}
	return false;
}

void setPrecision(vector<double> &precision, double r, double p){

	int i=0;
	double last;

	for(; i < 11; i++){
		if(precision[i] == 0){
			break;
		}
		else{
			last = precision[i];
		}
	}

	if(p > last && i > 0){
		p = last;
	}

	for(; i*10 <= r*100 && i < 11; i++){
//		cout << i*10 << "  " << r*100 << endl;
		precision[i] = p;
	}
}

void calcPrecisionRecall(string path, vector <string> &relevantList, vector<double> &precision){

	FILE *fp = fopen(path.c_str(), "r");
	if(fp == NULL){
		printf("Error openning the file.\n");
		exit(EXIT_FAILURE);
	}

	char url[1000];
	size_t num_relevant = relevantList.size();
	size_t urlCount = 0, relCount = 0;

	while(fscanf(fp, "%[^\n]\n", url) > 0){
		urlCount++;

		if(searchRelevant(url, relevantList)){
			relCount++;
			double r = (double)relCount/(double)num_relevant;
			double p = (double)relCount/(double)urlCount;
//			cout << "Recall: " << r << " Precision: " << p << endl;

			setPrecision(precision, r, p);

			if(relCount == num_relevant)
				break;
		}
	}

	fclose(fp);

}

int main(int argc, char **argv){

	oneurl curl;
	string rDir = string(argv[1]);
	string qDir = string(argv[2]);
	string qFilePath = string(argv[3]);

	vector<string> queryList;

	loadLines(qFilePath, queryList);

	for(size_t i=0; i<queryList.size(); i++){

		string q = queryList[i];

		size_t j = q.find_first_of(" ");

		while (j != string::npos) { // While our position in the sting is in range.
			q[j] = '-'; // Change the character at position.
			j = q.find_first_of(" ", j + 1); // Relocate again.
		}


//		printf("\\subfigure[%s]{\\includegraphics[width=.31\\linewidth]{images/graphics/%s.jpg}}\\hfill\n", queryList[i].c_str(), q.c_str());




		vector <string> relevantList;
		loadLines(rDir + queryList[i], relevantList);

		vector<double> precision(11, 0);

		calcPrecisionRecall(qDir + queryList[i], relevantList, precision);

		cout << queryList[i] << endl;

		for(int j=0; j<11; j++){
			cout << j*10 << ";" << precision[j] << endl;
		}

		cout << endl;
	}

	return 0;
}
