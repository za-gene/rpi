//#include <stdio.h>
//#include <string.h>
//#include "pico/stdlib.h"
//#include "hardware/spi.h"
#include "hardware/gpio.h"

#include "blinkt.h"

//#define clockPin 14
//#define dataPin  15

#define OUTPUT 	GPIO_OUT
#define INPUT 	GPIO_IN
#define HIGH	1
#define LOW	0

typedef uint8_t u8;

static u8 _dataPin, _clockPin;

static void pinMode(u8 pin, u8 dir)
{
	gpio_init(pin);
	gpio_set_dir(pin, dir);
}

static void digitalWrite(u8 pin, u8 value)
{
	if(value == 0)
		gpio_put(pin, 0);
	else
		gpio_put(pin, 1);
}

void blinkt_init(uint8_t dataPin, uint8_t clockPin)
{
	pinMode(dataPin , OUTPUT);
    _dataPin = dataPin;
	pinMode(clockPin, OUTPUT);
    _clockPin = clockPin;
	digitalWrite(dataPin , LOW);
	digitalWrite(clockPin, LOW);
}




const int numLEDs = 8;
static uint8_t pixels[numLEDs * 3];



/*
static void delay(int ms)
{
	sleep_ms(ms);
}
*/


static void spi_out(uint8_t n) {
	for (uint8_t i = 8; i--; n <<= 1) {
		if (n & 0x80) digitalWrite(_dataPin, HIGH);
		else          digitalWrite(_dataPin, LOW);
		digitalWrite(_clockPin, HIGH);
		digitalWrite(_clockPin, LOW);
	}
}

/** After calling blinkt_show():
  sleep_ms(100);
  */

void blinkt_show() 
{
	uint8_t i;
	uint8_t n = numLEDs;
	uint8_t *ptr = pixels;


	for (i = 0; i < 4; i++) spi_out(0x00); // 4 byte start-frame marker

	do {                               // For each pixel...
		spi_out(0xFF);                   //  Pixel start
		for (i = 0; i < 3; i++) spi_out(*ptr++); // Write R,G,B
	} while (--n);

	for (i = 0; i < ((numLEDs + 15) / 16); i++) spi_out(0xFF); // end-frame marker
}

/** pos in range 0..7 incl */
void blinkt_set_pixel_colour(uint8_t pos , uint8_t r, uint8_t g, uint8_t b) 
{

	pos *= 3;
	pixels[pos++] = b;
	pixels[pos++] = g;
	pixels[pos] = r;
}


/*
   void loop() {
   static uint8_t red = 0;
   red = 10 - red; // switch between on and off
   for (int i = 0; i < 8; ++i) {
   show();
   delay(100);
   setPixelColour(i, red, 0, 0);
   }

   }



   int main() 
   {
   stdio_init_all();

   pinMode(dataPin , OUTPUT);
   pinMode(clockPin, OUTPUT);
   digitalWrite(dataPin , LOW);
   digitalWrite(clockPin, LOW);
   show();

   for(;;) loop();
   return 0;
   }
   */
