//#include <fstream>
//#include <sstream>
//#include <iostream>
#include <string>

#include <math.h>
#include <bcm2835.h>

#include "mcp4921.h"

using namespace std;

typedef uint16_t u16;


mcp4921 mcp;

#define SCALE	16
#define SAMPLES (4096/SCALE)
//u16 sines[SAMPLES];


int main()
{

	// initialise
	const double freq = 200;

	/*
	double lo = 1.0/3.3 * hi; // 1V
	lo = 0;
	for (int i = 0; i < SAMPLES; i++) {
		double v = sin((double) i * 2.0 * 3.1412 / SAMPLES);
		v = (v+1.0)*(hi-lo)/2.0 + lo;
		sines[i] = v;
		printf("%d %d\n", i, sines[i]);
	}
	*/

	int delay = 1'000'000 / freq / SAMPLES;
	printf("delay=%d\n", delay);	
	while(1)  {
		for (int i = 0; i < SAMPLES; i++) {
			mcp.write(i*SCALE);
			bcm2835_delayMicroseconds(delay);
		}
	}

	return 0;
}

