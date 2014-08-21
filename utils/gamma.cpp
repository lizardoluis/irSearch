#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

vector<bool> encode(unsigned int num) {

	vector<bool> bitvector;

	// Increments to code 0
	num++;

	// First part
	unsigned int un = floor(log2(num));
	for (unsigned int i = 0; i < un; ++i) {
		bitvector.push_back(0);
	}
	bitvector.push_back(1);

	// Second part
	unsigned int x = num - pow(2, un);
	unsigned int mask = 1 << un - 1;

	for (unsigned int i = 0; i < un; ++i) {
		bitvector.push_back(x & mask);
		mask = mask >> 1;
	}

	return bitvector;
}

unsigned int decode(vector<bool> &bitvector) {

	unsigned int cu = 0;

	// Extract the fist part
	while (bitvector[cu] == 0 && cu < bitvector.size()) {
		cu++;
	}

	// Extract the second part
	unsigned int cb = 0;
	for (unsigned int i = cu + 1; i < bitvector.size(); i++) {
		unsigned int bit = bitvector[i];
		cb = cb << 1;
		cb = cb | bit;
	}

	// Decrements before returning to decode 0
	return pow(2, cu) + cb - 1;
}

int main(int argc, char** argv) {

	for (int i = 1; i <= 20; i++) {
		cout << i << " - ";
		vector<bool> bitvector = encode(i);
		for(int i=0; i<bitvector.size(); i++){
				if(bitvector[i] == true)
					cout << 1 << " ";
				else
					cout << 0 << " ";
			}


		int num = decode(bitvector);
		cout << " - " << num << endl;
	}

	//	encode(9);

	return 0;
}
