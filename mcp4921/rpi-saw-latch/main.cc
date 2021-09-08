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

#define PWM_PIN RPI_GPIO_P1_12 // physical pin 18

void pwm_init(void)
{
#define PWM_CHANNEL 0
#define PWM_FRAMERATE 16'000
#define PWM_RANGE (1'200'000/PWM_FRAMERATE)
	bcm2835_gpio_fsel(PWM_PIN, BCM2835_GPIO_FSEL_ALT5);
	bcm2835_pwm_set_clock(BCM2835_PWM_CLOCK_DIVIDER_16); // produces a clock rate of 1.2MHz
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

