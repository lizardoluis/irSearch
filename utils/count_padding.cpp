#include <cstdio>
#include <string>
#include <climits>

using namespace std;

int main(int argc, char** argv) {

	string binPath = argv[1];

	FILE *fp = fopen(binPath.c_str(), "rb");

	if(fp == NULL){
		printf("Error when opening the file\n");
		return 1;
	}

	unsigned int t, tb, d, db, fb;

	size_t count=0;
	size_t line =1;

	while(!feof(fp)){

		fread(&tb, sizeof(unsigned int), 1, fp);
		fread(&db, sizeof(unsigned int), 1, fp);
		fread(&fb, sizeof(unsigned int), 1, fp);

		if(tb == UINT_MAX && db == UINT_MAX && fb == UINT_MAX){
			count ++;
		}
		line ++;

	}

	fclose(fp);

	printf("Padding %ld\n", count);

	return 0;
}
