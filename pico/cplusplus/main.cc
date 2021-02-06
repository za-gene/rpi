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

	//throw 666;
	return 0;
}
