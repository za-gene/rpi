#include <lfb.h>
#include <stdio.h>
#include <string.h>

#include <sd.h>
#include <tat.h>
#include <uart0.h>


void kernel_main()
{
	uart0_init_as_stdio();

	bcm2835_init();
	// PWM base frequency is 19.2MHz
	// freq = 19'200'000/DIVIDER/RANGE = 19'200'000/4/256 = 18750Hz
	bcm2835_pwm_set_clock(BCM2835_PWM_CLOCK_DIVIDER_16);
	bcm2835_pwm_set_clock(BCM2835_PWM_CLOCK_DIVIDER_4);
	bcm2835_pwm_set_mode(PWM_CHANNEL, 1, 1);
	bcm2835_pwm_set_range(PWM_CHANNEL, RANGE);



	puts("Initialising card");
	sd_init();
	puts("Mounting partition...");
	tat_mount();

	int fs = tat_open("song");
	assert(fd!=-1);

	int size = tat_size();
	uchar* data = malloc(size);
	assert(data);

	int idx = 0;
	while(1) {
		uchar vol = data[idx++];
		if(idx == size) idx = 0;
		bcm2835_pwm_set_data(PWM_CHANNEL, data);
		bcm2835_delayMicroseconds(125);
	}
	bcm2835_close();
}
