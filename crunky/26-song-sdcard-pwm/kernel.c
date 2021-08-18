#include <assert.h>
#include <stdio.h>
#include <bcm2835.h>
#include <../pico/fat32.h>
#include <ctype.h>
#include <string.h>

// PWM output on RPi Plug P1 pin 12 (which is GPIO pin 18)
// in alt fun 5.
// Note that this is the _only_ PWM pin available on the RPi IO headers
#define PIN RPI_GPIO_P1_12
// and it is controlled by PWM channel 0
#define PWM_CHANNEL 0
// This controls the max range of the PWM signal
#define RANGE 256


uint8_t* song;
uint32_t len;

bool file32_is_canonical(const char *filename)
{
	if(strlen(filename) != 11) return false;

	while(*filename) {
		char c = *filename;
		if(!(isupper(c) || c == ' ')) return false;;
		filename++;
	}
	return true;
}

bool fats32_initialised = false;

void fat32_soft_init(void)
{
	if(fats32_initialised) return;
	fats32_initialised = true;
	fat32_init();
}

uint8_t* file32_slurp(const char *filename, uint32_t *len, bool *found)
{
	*len = 0;
	*found = false;
	fat32_soft_init();
	file32_t file;
	char cooked_name[12];
	if(file32_is_canonical(filename))
		strcpy(cooked_name, filename);
	else
		canfile(cooked_name, filename);
	file32_init(&file, cooked_name);
	*found = file32_found(&file);
	if(!*found) return 0;
	*len = file32_size(&file);
	char* data = malloc(*len);
	size_t offset = 0;
	while(file32_read(&file, data + offset)) offset += 512;
	return data;
}

void test1(void)
{
	bool found;

	printf("strlen=%d\n", strlen("foo"));
	void test(const char *name) {
		printf("%s:%c\n", name, file32_is_canonical(name) ? 'y' : 'n');
	}

	test("readme.txt");
	test("README  TXT");
	test("REA.ME  TXT");


	void print(char* data, size_t len)
	{
		for(size_t i = 0; i< len; i++)
			putchar(*(data+i));
	}
	//uint8_t* foo1 = file32_slurp("lorem.txt", &len, &found);
	uint8_t* foo1 = file32_slurp("readme.txt", &len, &found);
	//uint8_t* foo1 = file32_slurp("LOREM   TXT", &len, &found);
	printf("found?%c\n", found? 'y' : 'n');
	print(foo1, len);

	uint8_t* foo2 = file32_slurp("readme.txt", &len, &found);
	printf("found?%c\n", found? 'y' : 'n');
	print(foo2, len);

	uint8_t* foo3 = file32_slurp("yuk.txt", &len, &found);
	printf("found?%c\n", found? 'y' : 'n');
	print(foo2, len);
	
	puts("bye");


	return;


}
void kernel_main(void)
{
	puts("\nPlaying song with PWM audio");
	bool found;
	song = file32_slurp("song.raw", &len, &found);
	printf("Song %s\n", found ? "found" : "unfound");
	assert(song);



	bcm2835_init();

	// Set the output pin to Alt Fun 5, to allow PWM channel 0 to be output there
	bcm2835_gpio_fsel(PIN, BCM2835_GPIO_FSEL_ALT5);

	// PWM base frequency is 19.2MHz
	// freq = 19'200'000/DIVIDER/RANGE = 19'200'000/4/256 = 18750Hz
	bcm2835_pwm_set_clock(BCM2835_PWM_CLOCK_DIVIDER_16);
	bcm2835_pwm_set_clock(BCM2835_PWM_CLOCK_DIVIDER_4);
	bcm2835_pwm_set_mode(PWM_CHANNEL, 1, 1);
	bcm2835_pwm_set_range(PWM_CHANNEL, RANGE);


	/*
	uint32_t offset = 0;
	while(file32_read(&file, song + offset))
		offset += 512;
		*/
	puts("Song finished reading.");

	// Vary the PWM m/s ratio between 1/RANGE and (RANGE-1)/RANGE
	int idx = 0;
	//int data = 1;
	while (1)
	{
		unsigned char data = song[idx++];
		if(idx == len) idx = 0;

		bcm2835_pwm_set_data(PWM_CHANNEL, data);
		bcm2835_delayMicroseconds(1000000/16000);
	}

	bcm2835_close();
}
