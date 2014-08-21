#include <cstdio>
#include <string>

using namespace std;

int main(int argc, char** argv) {

	string binPath = argv[1];

	FILE *fp = fopen(binPath.c_str(), "rb");

	if(fp == NULL){
		printf("Error when opening the file\n");
		return 1;
	}

	unsigned int t, tb, d, db, fb;
	fread(&tb, sizeof(unsigned int), 1, fp);
	fread(&db, sizeof(unsigned int), 1, fp);
	fread(&fb, sizeof(unsigned int), 1, fp);

	while(!feof(fp)){
		t = tb; d = db;

		fread(&tb, sizeof(unsigned int), 1, fp);
		fread(&db, sizeof(unsigned int), 1, fp);
		fread(&fb, sizeof(unsigned int), 1, fp);

		if(tb < t || (tb == t && db < d)){
			printf("Invalid file\n");
			return 1;
		}
	}

	fclose(fp);

	printf("Valid file\n");

	return 0;
}
