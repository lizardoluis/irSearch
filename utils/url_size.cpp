#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]){

	FILE *fp = fopen(argv[1], "r");

	char termStr[51], urlStr[2048];
	size_t docNumber, numLinks;
	unsigned int sourceDocId;

	size_t size = 0, num_url=0;

	while (!feof(fp)) {
		fscanf(fp, "%u %lu\n", &sourceDocId, &numLinks);

		for (size_t i = 0; i < numLinks; i++) {
			fscanf(fp, "%2047s\n", urlStr);
			string url = string(urlStr);

			size += url.length();
			num_url ++;

			while (fscanf(fp, "%50s", termStr) > 0) {
				string term = string(termStr);
				if (term == ">#")
					break;
			}
		}
	}

	cout << "Media: " << size / num_url << endl;
	cout << "Num URLS " << num_url << endl;
	cout << "size " << size << endl;


	fclose(fp);

	return 0;
}
