#include <bitset>
#include <cassert>
#include <iostream>

#include <wiringPi.h>
#include <wiringPiSPI.h>


typedef unsigned char uchar;
typedef unsigned int uint;
typedef uint16_t u16;

using namespace std;

void method1(){
	uchar data[2];
		wiringPiSPIDataRW(0, data, 2); // discard
	auto ms = micros();
	//u16 v;
	while(1) {
	//for(int i = 0; i<100; ++i) {
		wiringPiSPIDataRW(0, data, 2);
		u16 v = ((int) data[0] << 8) + (int) data[1] ;
		//cout <<  (int) data[0] << " " << (int) data[1] << "  " << v << "\n";
		//cout << std::bitset<16>(micros()) << std::bitset<16>(v);
		cout << (micros() -ms) << "\t" << v << "\n";
	}
	ms = micros() - ms;
	//cout << "100 reads took " << ms << "us\n";

}

void method2()
{
	uchar data[200];
	wiringPiSPIDataRW(0, data, sizeof(data));
	int semi = sizeof(data)/2;
	for(int i=0; i<semi; ++i) {
		u16 val = ((int) data[i*2] << 8) + (int) data[i+2+1];
		//val 	<<=8 + data[i*2];
		cout << val << "\n";
	}



}
int main()
{
	int fd = wiringPiSPISetup(0, 1000000);
	assert(fd != -1);

	//cout << sizeof(uint) << "\n"; // s/b 4 bytes on 32-bit m/c
	method1();

}
