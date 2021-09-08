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

//#define SCALE	16
//#define SAMPLES (4096/SCALE)

#define PWM_PIN 	RPI_GPIO_P1_12 // physical pin 18
#define PWM_FRAMERATE 	16'000
#define PWM_DIVIDER 	BCM2835_PWM_CLOCK_DIVIDER_2
#define PWM_RANGE 	(1'200'000 * 16 /PWM_FRAMERATE / PWM_DIVIDER)

void pwm_init(void)
{
#define PWM_CHANNEL 0
	bcm2835_gpio_fsel(PWM_PIN, BCM2835_GPIO_FSEL_ALT5);
	bcm2835_pwm_set_clock(PWM_DIVIDER);
	bcm2835_pwm_set_mode(PWM_CHANNEL, 1, 1);
	bcm2835_pwm_set_range(PWM_CHANNEL, PWM_RANGE);
	bcm2835_pwm_set_data(PWM_CHANNEL, PWM_RANGE/2);
}

int main()
{
	if (!bcm2835_init()) return 1;
	pwm_init();

	bcm2835_gpio_ren(PWM_PIN); // enable rising edge detect
	//bcm2835_gpio_ren(PWM_PIN); // enable rising edge detect


	// initialise
	const double freq = 400;
	const double dy = freq / PWM_FRAMERATE ;
	printf("dy=%f\n", dy);

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

	//int delay = 1'000'000 / freq / SAMPLES;
	//printf("delay=%d\n", delay);
	double y = 0;
	while(1)  {
		if(bcm2835_gpio_eds(PWM_PIN)) { // did we detect an event (rising edge)
			bcm2835_gpio_set_eds(PWM_PIN); // reset the event flag
			mcp.write(y*4095);
			y += dy;
			if(y>=1) y = 0;
			//bcm2835_delayMicroseconds(1);		
			//if(++i >= SAMPLES) i = 0;
		}
	}

	return 0;
}

