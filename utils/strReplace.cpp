#include <string>
#include <cstdio>

using namespace std;


void replaceStrChar(string &str, string replace, char ch) {

	// set our locator equal to the first appearance of any character in replace
	size_t i = str.find_first_of(replace);

	while (i != string::npos) { // While our position in the sting is in range.
		str[i] = ch; // Change the character at position.
		i = str.find_first_of(replace, i + 1); // Relocate again.
	}
}

int main(){

	string str("luis\n eduardo\n oliveira\n");

	string r("\n");

	replaceStrChar(str, r, ' ');

	printf("-%s-\n", str.c_str());

return 0;

}
