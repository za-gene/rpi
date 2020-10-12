// attempt to convert arduino library to bare metal

#include <stdbool.h>

#include <stm8.h>
#include <millis.h>

// /home/pi/.arduino15/packages/sduino/hardware/stm8/0.5.0/libraries/I2C
// /home/pi/.arduino15/packages/sduino/hardware/stm8/0.5.0/STM8S_StdPeriph_Driver/src

//#include "I2C.h"

#define SID 0x3C // I2C 1306 slave ID ... for 64 height display.
#define HEIGHT 64
#define WIDTH 128

#define REG(addr) *(volatile u8*)(addr)

#define I2C_CR1   REG(0x5210)
#define I2C_CR2   REG(0x5211)
#define I2C_FREQR   REG(0x5212)
#define I2C_OARL  REG(0x5213)
#define I2C_OARH  REG(0x5214)
#define I2C_DR    REG(0x5216)
#define I2C_SR1   REG(0x5217)
#define I2C_SR2   REG(0x5218)
#define I2C_SR3   REG(0x5219)
#define I2C_CCRL REG(0x521B)
#define I2C_CCRH REG(0x521C)
#define I2C_TRISER REG(0x521D)

//#define     I2C_OARH_ADDMODE (1<<7)               //  7 bit address mode.
//#define    I2C_OARH_ADDCONF (1<<6)               //  Docs say this must always be 1.

#define I2C_MAX_STANDARD_FREQ ((uint32_t)100000)

#undef I2C_CR1_PE

#define I2C_CR1_PE (1<<0)

#undef I2C_CR2_START 
#undef I2C_CR2_STOP 
#undef I2C_CR2_ACK


#define I2C_CR2_START (1<<0)
#define I2C_CR2_STOP (1<<1)
#define I2C_CR2_ACK (1<<2)

#undef I2C_SR1_SB
#undef I2C_SR1_RXNE
#undef I2C_SR1_TXE


#define I2C_SR1_SB (1<<0)
#define I2C_SR1_ADDR (1<<1)
#define I2C_SR1_BTF (1<<2)
#define I2C_SR1_RXNE (1<<6)
#define I2C_SR1_TXE (1<<7)

#define I2C_SR3_MSL (1<<0)


static void end_i2c_1(void)
{
	//while (!((I2C_SR1 & (I2C_SR1_TXE | I2C_SR1_BTF)) == (I2C_SR1_TXE | I2C_SR1_BTF)));

	I2C_CR2 |= I2C_CR2_STOP;
	while(I2C_SR3 & I2C_SR3_MSL);
}

void write_i2c_byte_1(uint8_t dat)
{
	I2C_DR = dat;
	while (!(I2C_SR1 & I2C_SR1_TXE));
}

static void begin_i2c_1(uint8_t slave_id, bool read)
{
	I2C_CR2 |= I2C_CR2_START;  // send start sequence
	while (!(I2C_SR1 & I2C_SR1_SB)); // EV5
	I2C_DR = (slave_id << 1) + (read ? 1 : 0); // send the address and direction

	// EV6 ADDR=1, cleared by reading SR1 register, then SR3
	while (!(I2C_SR1 & I2C_SR1_ADDR));
	I2C_SR3;   // read SR3 to clear ADDR event bit

	I2C_CR2 |= I2C_CR2_ACK;  // set ACK
}



void send_cmd(u8 cmd) {
	begin_i2c_1(SID, false);
	write_i2c_byte_1(cmd);
	end_i2c_1();
}



void init_i2c_ori() {
	uint32_t OutputClockFrequencyHz = I2C_MAX_STANDARD_FREQ;
	//Serial_println_u(I2C_MAX_STANDARD_FREQ);
	uint8_t InputClockFrequencyMHz = 2; // 16;
	I2C_FREQR = InputClockFrequencyMHz;
	I2C_TRISER = InputClockFrequencyMHz + 1; // max rise time

	// set clock control frequency registers
	uint16_t speed = (uint16_t)((InputClockFrequencyMHz * 1000000) / (OutputClockFrequencyHz / 2));
	if (speed < (uint16_t)0x0004) speed = (uint16_t)0x0004; // must be at least 4
	I2C_CCRL = (uint8_t)speed;
	I2C_CCRH = (uint8_t)(speed >> 8);

	I2C_CR1 |= I2C_CR1_PE; // enable I2C
}

//I2C_FREQR_FREQ1 
#define I2C_OARH_ADDMODE (1<<7) // hmmm, goes to 10-bit addr
void init_i2c_1()
{
	I2C_FREQR = 0x2; // mc
	I2C_CCRL = 0x0A; // 100kHz
	I2C_OARH = I2C_OARH_ADDMODE; // 7-bit addressing
	I2C_CR1 |= I2C_CR1_PE;
}

#define SSD1306_BLACK 0   ///< Draw 'off' pixels
#define SSD1306_WHITE 1   ///< Draw 'on' pixels
#define SSD1306_INVERSE 2 ///< Invert pixels

#define SSD1306_MEMORYMODE 0x20          ///< See datasheet
#define SSD1306_COLUMNADDR 0x21          ///< See datasheet
#define SSD1306_PAGEADDR 0x22            ///< See datasheet
#define SSD1306_SETCONTRAST 0x81         ///< See datasheet
#define SSD1306_CHARGEPUMP 0x8D          ///< See datasheet
#define SSD1306_SEGREMAP 0xA0            ///< See datasheet
#define SSD1306_DISPLAYALLON_RESUME 0xA4 ///< See datasheet
#define SSD1306_DISPLAYALLON 0xA5        ///< Not currently used
#define SSD1306_NORMALDISPLAY 0xA6       ///< See datasheet
#define SSD1306_INVERTDISPLAY 0xA7       ///< See datasheet
#define SSD1306_SETMULTIPLEX 0xA8        ///< See datasheet
#define SSD1306_DISPLAYOFF 0xAE          ///< See datasheet
#define SSD1306_DISPLAYON 0xAF           ///< See datasheet
#define SSD1306_COMSCANINC 0xC0          ///< Not currently used
#define SSD1306_COMSCANDEC 0xC8          ///< See datasheet
#define SSD1306_SETDISPLAYOFFSET 0xD3    ///< See datasheet
#define SSD1306_SETDISPLAYCLOCKDIV 0xD5  ///< See datasheet
#define SSD1306_SETPRECHARGE 0xD9        ///< See datasheet
#define SSD1306_SETCOMPINS 0xDA          ///< See datasheet
#define SSD1306_SETVCOMDETECT 0xDB       ///< See datasheet

#define SSD1306_SETLOWCOLUMN 0x00  ///< Not currently used
#define SSD1306_SETHIGHCOLUMN 0x10 ///< Not currently used
#define SSD1306_SETSTARTLINE 0x40  ///< See datasheet

#define SSD1306_EXTERNALVCC 0x01  ///< External display voltage source
#define SSD1306_SWITCHCAPVCC 0x02 ///< Gen. display voltage from 3.3V

#define SSD1306_RIGHT_HORIZONTAL_SCROLL 0x26              ///< Init rt scroll
#define SSD1306_LEFT_HORIZONTAL_SCROLL 0x27               ///< Init left scroll
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29 ///< Init diag scroll
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A  ///< Init diag scroll
#define SSD1306_DEACTIVATE_SCROLL 0x2E                    ///< Stop scroll
#define SSD1306_ACTIVATE_SCROLL 0x2F                      ///< Start scroll
#define SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3             ///< Set scroll range



// Issue single command to SSD1306, using I2C or hard/soft SPI as needed.
// Because command calls are often grouped, SPI transaction and selection
// must be started/ended in calling function for efficiency.
// This is a private function, not exposed (see ssd1306_command() instead).
void ssd1306_command1(uint8_t c) {
	begin_i2c_1(SID, false);
	write_i2c_byte_1(0);
	write_i2c_byte_1(c);
	end_i2c_1();
}

void send_u8_i2c(u8 c) {
	write_i2c_byte_1(c);
}

void ssd1306_commandList(const uint8_t *c, uint8_t n) {
	begin_i2c_1(SID, false);
	send_u8_i2c(0x00); // Co = 0, D/C = 0
	while (n--) send_u8_i2c(*c++);
	end_i2c_1();
}

void init1306(u8 vcs) {
	//clear1306();
	u8 vccstate = vcs;

	//i2caddr = addr ? addr : ((HEIGHT == 32) ? 0x3C : 0x3D);
	//init_i2c_1();

	uint8_t comPins = 0x02;
	u8 contrast = 0x8F;

	if ((WIDTH == 128) && (HEIGHT == 32)) {
		comPins = 0x02;
		contrast = 0x8F;
	} else if ((WIDTH == 128) && (HEIGHT == 64)) {
		comPins = 0x12;
		contrast = (vccstate == SSD1306_EXTERNALVCC) ? 0x9F : 0xCF;
	} else if ((WIDTH == 96) && (HEIGHT == 16)) {
		comPins = 0x2; // ada x12
		contrast = (vccstate == SSD1306_EXTERNALVCC) ? 0x10 : 0xAF;
	} else {
		// Other screen varieties -- TBD
	}



	u8 init1[] = {
		SSD1306_DISPLAYOFF,         // 0xAE
		SSD1306_SETDISPLAYCLOCKDIV, // 0xD5
		0x80, // the suggested ratio 0x80
		SSD1306_SETMULTIPLEX, // 0xA8
		HEIGHT - 1,
		SSD1306_SETDISPLAYOFFSET, // 0xD3
		0x0,                      // no offset
		SSD1306_SETSTARTLINE | 0x0, // line #0
		SSD1306_CHARGEPUMP,        // 0x8D
		(vccstate == SSD1306_EXTERNALVCC) ? 0x10 : 0x14,
		SSD1306_MEMORYMODE, // 0x20
		0x00, // 0x0 act like ks0108
		SSD1306_SEGREMAP | 0x1,
		SSD1306_COMSCANDEC,
		SSD1306_SETCOMPINS, comPins,
		SSD1306_SETCONTRAST, contrast,
		SSD1306_SETPRECHARGE, // 0xd9
		(vccstate == SSD1306_EXTERNALVCC) ? 0x22 : 0xF1,
		SSD1306_SETVCOMDETECT, // 0xDB
		0x40,
		SSD1306_DISPLAYALLON_RESUME, // 0xA4
		SSD1306_NORMALDISPLAY,       // 0xA6
		SSD1306_DEACTIVATE_SCROLL,
		SSD1306_DISPLAYON
	};
	ssd1306_commandList(init1, sizeof(init1));
}


void  low_level_test() {
	static const uint8_t  dlist1[] = {
		SSD1306_PAGEADDR,
		1,                      // Page start address
		1,                   // Page end (not really, but works here)
		SSD1306_COLUMNADDR, 0, 7
	};
	ssd1306_commandList(dlist1, sizeof(dlist1));
	begin_i2c_1(SID, false);
	send_u8_i2c(0x40);
	send_u8_i2c(0b10101010);
	end_i2c_1();
}


void main() {
	//init_millis();
	init_i2c_ori();
	init1306(SSD1306_SWITCHCAPVCC);
	low_level_test();
	while(1);
}
