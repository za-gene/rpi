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

void write_i2c_byte_1(uint8_t dat)
{
	while (!(I2C_SR1 & I2C_SR1_TXE));
	I2C_DR = dat;
}

void pause()
{
	for(u32 i = 0; i< 1000UL; i++) nop();
}

static void end_i2c_1()
{
	for(u8 i = 0; i< 10; i++) nop();
	I2C_CR2 |= I2C_CR2_STOP;
}




static void begin_i2c_write(uint8_t slave_id)
{

	I2C_CR2 |= I2C_CR2_ACK;  // set ACK
	I2C_CR2 |= I2C_CR2_START;  // send start sequence
	while (!(I2C_SR1 & I2C_SR1_SB)); // EV5
	here();
	I2C_DR = (slave_id << 1); 

	// EV6 ADDR=1, cleared by reading SR1 register, then SR3
	while (!(I2C_SR1 & I2C_SR1_ADDR));
	I2C_SR3;   // read SR3 to clear ADDR event bit

}


void init_i2c_1() {
	I2C_CR1 &= ~I2C_CR1_PE; // disable I2C
	uint32_t OutputClockFrequencyHz = I2C_MAX_STANDARD_FREQ;
	uint8_t InputClockFrequencyMHz = 2; // 16;
	InputClockFrequencyMHz  = 2;
	I2C_FREQR = InputClockFrequencyMHz;
	I2C_TRISER = InputClockFrequencyMHz + 1; // max rise time

	uint16_t speed = (uint16_t)((InputClockFrequencyMHz * 1000000) / (OutputClockFrequencyHz / 2));
	speed = 4;
	I2C_CCRL = (uint8_t)speed;
	I2C_CCRH = (uint8_t)(speed >> 8);


	I2C_CR1 |= I2C_CR1_PE; // enable I2C
}



// Issue single command to SSD1306, using I2C or hard/soft SPI as needed.
// Because command calls are often grouped, SPI transaction and selection
// must be started/ended in calling function for efficiency.
// This is a private function, not exposed (see ssd1306_command() instead).
void ssd1306_command1(uint8_t c) {
	begin_i2c_write(SID);
	write_i2c_byte_1(0x80);
	write_i2c_byte_1(c);
	end_i2c_1();
}

void send_u8_i2c(u8 c) {
	write_i2c_byte_1(c);
}

void ssd1306_commandList(const uint8_t *c, uint8_t n) 
{
	while (n--) ssd1306_command1(*c++);
}

void send_datum(u8 val)
{
	begin_i2c_write(SID);
	send_u8_i2c(0x40);
	send_u8_i2c(val);
	end_i2c_1();
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

	begin_i2c_write(SID);
	send_u8_i2c(0x40);
	for(int i = 0; i<128*pages; i++)
		send_u8_i2c(0);
	end_i2c_1();


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
	begin_i2c_write(SID);
	send_u8_i2c(0x40);
	//send_u8_i2c(val);
	int offset = n*5;
	for(int i =offset; i< offset + 5; i++)
		send_u8_i2c(digital_font5x7_123[i]);
	end_i2c_1();
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


#if 0
return;

u8 letterP[] = {0x00, 0x7F, 0x09, 0x09, 0x09, 0x06};
//u8 letterP[] = {0xFF, 0x7F, 0x09, 0x09, 0x09, 0x06};
//for(int i = 0; i<1024; i++)
//	send_datum(0);

for(int i = 0; i<6; i++)
send_datum(letterP[i]);

//for(int i = 0; i<6; i++)
//	send_datum(letterP[i]);
#endif



void gpio_mode_out_drain(u8 pin)
{
#if 1
	pinMode(pin, OUTPUT);
#else
	PORT_t* port = pin_to_port(pin);
	u8 pos = pin_to_pos(pin);
	port->DDR |= (1<< pos); // direction is output
#endif
}


void main() {
#if 0
	gpio_mode_out_drain(PB4);
	gpio_mode_out_drain(PB5);
#endif
	pinMode(PA1, OUTPUT);
	pinMode(PA2, OUTPUT);
	pinMode(PD6, OUTPUT);

	init_millis();
	init_i2c_1(); // this completes
	//here();
	//while(1);
	//init1306(SSD1306_SWITCHCAPVCC); // this completes
	init1306();

	//for(u32 i = 0; i < 5000; i++) nop();
	//send_cmd(0xAE); // turn display off - which doesn't seem to work

	//gpio_write(PD6, 1); // check that we make it to end
	//here();

	int i = 0;
	while(1) {
		home();
		draw_num(i++);
		delay_millis(1000);
	}
}
