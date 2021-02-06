#include <string>
#include <vector>
#include "pico/stdlib.h"


int main() {
	stdio_init_all();

	//sleep_ms(10000);
	std::string str("im a string");
	puts(str.c_str());

	std::vector<int> vec;
	vec.push_back(22);

	try {
		throw 666;
		puts("this is never printed");
	} catch (int e) {
		printf("Caught error %d\n", e);
	}


	puts("Halting successfully");
	while(1);

	return 0;
}
