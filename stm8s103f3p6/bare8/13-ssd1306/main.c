#include <stdbool.h>

#include <gpio.h>
#include <stm8.h>
#include <millis.h>
#include <i2c.h>
#include <ssd1306.h>



#if 1 
#define HEIGHT 32
#else
#define HEIGHT 64
#endif

#define WIDTH 128
#define SID 0x3C
const u8 pages = HEIGHT/8-1;

void check()
{
	if(I2C_SR2 & IC2_SR2_AF) digitalWrite(PA2, 1);
	if(I2C_SR2 & IC2_SR2_BERR) digitalWrite(PA1, 1);
}

void here()
{
	digitalWrite(PD6, 1); // check that we make it here
}


void pause()
{
	for(u32 i = 0; i< 1000UL; i++) nop();
}







// Issue single command to SSD1306, using I2C or hard/soft SPI as needed.
// Because command calls are often grouped, SPI transaction and selection
// must be started/ended in calling function for efficiency.
// This is a private function, not exposed (see ssd1306_command() instead).
void ssd1306_command1(uint8_t c) {
	begin_i2c_trans(SID);
	write_i2c_byte(0x80);
	write_i2c_byte(c);
	end_i2c_trans();
}



void ssd1306_commandList(const uint8_t *c, uint8_t n) 
{
	while (n--) ssd1306_command1(*c++);
}

void send_datum(u8 val)
{
	begin_i2c_trans(SID);
	write_i2c_byte(0x40);
	write_i2c_byte(val);
	end_i2c_trans();
}


void triplet(u8 v1, u8 v2, u8 v3)
{
	ssd1306_command1(v1);
	ssd1306_command1(v2);
	ssd1306_command1(v3);
}


void clr_scr()
{
	// seems to work for both screens
	// not sure how efficient it is, though
	int pages = HEIGHT/8;
	triplet(SSD1306_PAGEADDR, 0, pages-1);
	triplet(SSD1306_COLUMNADDR, 0, 127);

	begin_i2c_trans(SID);
	write_i2c_byte(0x40);
	for(int i = 0; i<128*pages; i++)
		write_i2c_byte(0);
	end_i2c_trans();


}

void init1306() {
	u8 comPins = 0x12;
	if(HEIGHT==32) comPins = 0x02;


	u8 init1[] = {
		SSD1306_DISPLAYOFF,         // 0xAE
		SSD1306_SETDISPLAYCLOCKDIV, // 0xD5
		0x80, // the suggested ratio 0x80
		//SSD1306_SETDISPLAYOFFSET, // 0xD3
		//0x0,                      // no offset
		SSD1306_SETSTARTLINE | 0x0, // line #0
		SSD1306_CHARGEPUMP,        // 0x8D
		0x14,
		SSD1306_MEMORYMODE, 0, // 0x20, write across then down
		SSD1306_SEGREMAP | 0x1,
		SSD1306_COMSCANDEC,
		SSD1306_SETCOMPINS, comPins, // 0xDA
		SSD1306_SETPRECHARGE, // 0xd9
		0xF1,
		SSD1306_SETVCOMDETECT, // 0xDB
		0x40,
		//SSD1306_DISPLAYALLON_RESUME, // 0xA4
		0xA4, // Entire display on, output follows RAM content
		//SSD1306_NORMALDISPLAY,       // 0xA6
		//SSD1306_DEACTIVATE_SCROLL,
		SSD1306_SETMULTIPLEX, HEIGHT-1, // 0xA8 
		SSD1306_DISPLAYON // 0xAF
	};
	ssd1306_commandList(init1, sizeof(init1));
	clr_scr();
}

const uint8_t digital_font5x7_123[] =
{
	0x36, 0x41, 0x41, 0x36, 0x00, // char '0' (0x30/48)
	0x00, 0x00, 0x00, 0x36, 0x00, // char '1' (0x31/49)
	0x30, 0x49, 0x49, 0x06, 0x00, // char '2' (0x32/50)
	0x00, 0x49, 0x49, 0x36, 0x00, // char '3' (0x33/51)
	0x06, 0x08, 0x08, 0x36, 0x00, // char '4' (0x34/52)
	0x06, 0x49, 0x49, 0x30, 0x00, // char '5' (0x35/53)
	0x36, 0x49, 0x49, 0x30, 0x00, // char '6' (0x36/54)
	0x00, 0x01, 0x01, 0x36, 0x00, // char '7' (0x37/55)
	0x36, 0x49, 0x49, 0x36, 0x00, // char '8' (0x38/56)
	0x06, 0x49, 0x49, 0x36, 0x00, // char '9' (0x39/57)
	0x00, 0x00, 0x00, 0x00, 0x00, // 10 space
};



void draw_digit(u8 n)
{
	begin_i2c_trans(SID);
	write_i2c_byte(0x40);
	//send_u8_i2c(val);
	int offset = n*5;
	for(int i =offset; i< offset + 5; i++)
		write_i2c_byte(digital_font5x7_123[i]);
	end_i2c_trans();
}

void home()
{
	static const uint8_t  dlist1[] = {
		SSD1306_PAGEADDR,
		0,                      // Page start address
		3,                   // Page end (not really, but works here)
		SSD1306_COLUMNADDR, 0, 127
	};
	ssd1306_commandList(dlist1, sizeof(dlist1));
}


void draw_num(int n)
{
	char str[] = "0000000";
	const u8 len = sizeof(str);
	u8 i;
	for(i = 0; i<len; i++) {
		str[len-i-1] = (char)(n % 10) + '0';
		n = n/10;
	}


	bool allow_spaces = true;
	for(i = 0; i<len; i++) {
		u8 n = str[i] - '0';

		if(i+1 == len) { allow_spaces = 0;}
		if(allow_spaces) {
			if(n==0) {
				n=10; // space
			} else {
				allow_spaces = false;
			}
		}

		draw_digit(n);
	}
}





void main() {
	pinMode(PA1, OUTPUT);
	pinMode(PA2, OUTPUT);
	pinMode(PD6, OUTPUT);

	init_millis();
	init_i2c();
	init1306();

	int i = 0;
	while(1) {
		home();
		draw_num(i++);
		delay_millis(1000);
	}
}
