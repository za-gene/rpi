//#include <fstream>
//#include <sstream>
#include <iostream>
#include <string>

#include <bcm2835.h>

#include "mcp4921.h"

using namespace std;

mcp4921 mcp;


int main()
{

	string line;

	while(1) {
		printf("Enter level: 0-4095");
		try {
			getline(cin, line);
			int level = stoi(line);
			if(level<0 || level > 4095) throw std::invalid_argument("Out of bounds");
			mcp.write(level);
			bcm2835_delayMicroseconds(125);
		} catch (const std::invalid_argument& e) {
			cerr << "Invalid argument. " << e.what() << "\n";
		}
	}

	return 0;
}

