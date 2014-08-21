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

	unsigned int t, d, f;

	while(!feof(fp)){

		fread(&t, sizeof(unsigned int), 1, fp);
		fread(&d, sizeof(unsigned int), 1, fp);
		fread(&f, sizeof(unsigned int), 1, fp);

		printf("%u %u %u\n", t, d, f);

	}

	fclose(fp);

	return 0;
}
