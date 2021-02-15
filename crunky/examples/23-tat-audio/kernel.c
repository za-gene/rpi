#include <assert.h>
//#include <lfb.h>
#include <stdio.h>
#include <string.h>

#include <bcm2835.h>
#include <sd.h>
#include <tat.h>
#include <uart0.h>

typedef unsigned char uchar;

#define PIN RPI_GPIO_P1_12
#define PWM_CHANNEL 0
#define RANGE 256


void kernel_main()
{
	uart0_init_as_stdio();

	bcm2835_init();
	bcm2835_gpio_fsel(PIN, BCM2835_GPIO_FSEL_ALT5);
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

	int fd = tat_open("song");
	assert(fd!=-1);

	int size = tat_size();
	uchar* data = malloc(size);
	assert(data);

	puts("Reading in song");
	int block = 0;
	int nread;
	while(nread = tat_read(data + block*512)) block++;

	int idx = 0;
	while(1) {
		if(idx== 0) puts("Starting song");
		uchar vol = data[idx++];
		//printf("%d\n", vol);
		if(idx == size) idx = 0;
		bcm2835_pwm_set_data(PWM_CHANNEL, vol);
		bcm2835_delayMicroseconds(125);
	}
	free(data);
	bcm2835_close();
}
