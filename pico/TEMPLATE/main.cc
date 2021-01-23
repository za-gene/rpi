#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"

#define MISO 16 // (physical pin 21)
#define CS   17 // (.. pin 22)
#define SCK  18 // (.. pin 24)
#define MOSI 19 // (.. pin 25)
#define RS   20 // (.. pin 26)
#define SPI_PORT spi0

/*
 * power connections:
 * VIN pin of LCD should go to Pico pin 40 (VBUS). being 5V
 * GND of LCD to any GND pin of Pico
 */

#define LOW 0
#define HIGH 1


void digitalWrite(int pin, int val)
{
	if(val)
		gpio_put(pin, 1);
	else
		gpio_put(pin, 0);
}





/* rs_val = LOW to send command, HIGH to send ASCII char 
*/
void sendByte(int rs_val, uint8_t val) {
	digitalWrite(RS, rs_val);
	digitalWrite(CS, LOW);
	//SPI.transfer(val);
	spi_write_blocking(SPI_PORT, &val, 1);
	digitalWrite(CS, HIGH);  
	sleep_ms(60);
}

int main() {
	// This example will use SPI0 at 0.5MHz.
	spi_init(SPI_PORT, 500 * 1000);
	gpio_set_function(MISO, GPIO_FUNC_SPI);
	gpio_set_function(SCK, GPIO_FUNC_SPI);
	gpio_set_function(MOSI, GPIO_FUNC_SPI);

	// Chip select is active-low, so we'll initialise it to a driven-high state
	gpio_init(CS);
	gpio_set_dir(CS, GPIO_OUT);
	gpio_put(CS, 1);

	// put your setup code here, to run once:
	//pinMode(CS, OUTPUT);
	//digitalWrite(CS, HIGH);
	gpio_init(RS);
	//pinMode(RS, OUTPUT);
        gpio_set_dir(RS, GPIO_OUT);

	//SPI.begin();
	//SPI.setBitOrder(MSBFIRST);
	//Serial.begin(115200);

	constexpr auto contrast = 0x70  | 0b1000; // from 0x7C
	constexpr auto display = 0b1111; // ori 0x0F
	uint8_t cmds[] = {0x39, 0x1D, 0x50, 0x6C, contrast , 0x38, display, 0x01, 0x06};
	for(int i=0; i<sizeof(cmds); ++i) sendByte(LOW, cmds[i]);
	//Serial.println(sizeof(cmds));

	// now send some intersting output
	uint8_t msg[] = {'H', 'E', 'L', 'L', 'O'};
	for(int i=0; i<sizeof(msg); ++i) sendByte(HIGH, msg[i]);
	//Serial.println(sizeof(msg));


	for(;;);

	return 0;
}

