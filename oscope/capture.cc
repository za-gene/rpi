#include <cassert>
#include <iostream>

#include <wiringPi.h>
#include <wiringPiSPI.h>


typedef unsigned char uchar;
typedef unsigned int uint;

using namespace std;

int main()
{
	int fd = wiringPiSPISetup(0, 1000000);
	assert(fd != -1);

	uchar data[20];
	uint ms = micros();
	for(int i = 0; i<100; ++i) {
		int val = wiringPiSPIDataRW(0, data, 20);
	}
	ms = micros() - ms;
	cout << "100 reads took " << ms << "us\n";


}
