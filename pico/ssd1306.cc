/** @addtogroup ssd1306

Use an OLED display.

`ssd1306_display_cell()` takes about 117us to transfer a cell at a transfer rate of 1Mbps. 
It is intended to be used in the main program loop where you don't want the transfer
blocking for too long. See project `ssd1306` for an example of its use.

The repo from which this code is derived used "vetical addressing mode", which I found to be inconvenient.
I have therefore used horizontal addressing mode. This is the default, but you can set it manually
as follows:
@code
	write_cmd(SET_MEM_ADDR); // 0x20
	//write_cmd(0b01); // vertical addressing mode
	write_cmd(0b00); // horizontal addressing mode

	write_cmd(SET_COL_ADDR); // 0x21
	write_cmd(0);
	write_cmd(127);

	write_cmd(SET_PAGE_ADDR); // 0x22
	write_cmd(0);
	write_cmd(pages()-1);
@endcode
see also: function reset_addressing().

*/

//#include <cstdint>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include "ssd1306.h"


extern "C" const uint8_t ssd1306_font6x8[];


//typedef uint8_t uint8_t;

//#define I2C_PORT i2c0

auto i2c_port = &i2c0_inst;

//#define OLED_128x32

/*
#ifdef OLED_128x32
const uint8_t height = 32;
#else
const uint8_t height = 64;
#endif
*/

static uint8_t height = 32;
//const uint8_t SID = (height == 64) ? 0x3C : 0x3D; // different height displays have different addr
const uint8_t SID = 0x3C ; // different height displays have different addr
const uint8_t width = 128;

int pages() { return height/8; }

//uint8_t scr[pages*width+1]; // extra byte holds data send instruction
uint8_t scr[1025]; // being: 8 pages (max) * 128 width + 1 I2C command byte

/*
struct {
	uint8_t cmd = 0x40; // the data instruction
	uint8_t display[8][128];
} screen;
*/

void write_cmd(uint8_t cmd);

void fill_scr(uint8_t v)
{
	memset(scr, v, sizeof(scr));
}

void clear_scr(void)
{
	fill_scr(0);
}

void ssd1306_send_data(uint8_t* data, int nbytes)
{
	i2c_write_blocking(i2c_port, SID, data, nbytes, false);
}

void send2(uint8_t v1, uint8_t v2)
{
	uint8_t buf[2];
	buf[0] = v1;
	buf[1] = v2;
	ssd1306_send_data(buf, 2);
}

/** @brief send current cell to OLED

A "cell" is an 8x8 block.

The function keeps an internal position of the cell it wants to
write. This is independent of the screen cursor.

You would typically use this function in your main loop, where
it is important that functions don't block too long.

*/ 
void ssd1306_display_cell (void)
{
	//show_scr(); return;

	static int offset = 0;
	//if(offset == -1) return;
	uint8_t block[1+8]; // the data command, plus 8 column bytes
	block[0] = 0x40;
	memcpy(block+1, scr + offset + 1, 8);
	ssd1306_send_data(block, 9);
	offset += 8;
	if(offset >= width * pages())  {
		offset = 0;
		//offset = -1;
	}
}


/** @brief send screen to the display itself

Seems to take about 100ms at 100kps tfr rate, which is disappointing.


*/
void show_scr (void)
{
#if 0
	write_cmd(SET_MEM_ADDR); // 0x20
	//write_cmd(0b01); // vertical addressing mode
	write_cmd(0b00); // horizontal addressing mode

	write_cmd(SET_COL_ADDR); // 0x21
	write_cmd(0);
	write_cmd(127);

	write_cmd(SET_PAGE_ADDR); // 0x22
	write_cmd(0);
	write_cmd(pages()-1);
#endif

	scr[0] = 0x40; // the data instruction	
	int size = pages()*width +1;
	ssd1306_send_data(scr, size);
}



void write_cmd(uint8_t cmd) 
{ 
	send2(0x80, cmd);
}


void ssd1306_write_cmd(uint8_t cmd) { send2(0x80, cmd); }

void poweroff() { write_cmd(SET_DISP | 0x00); }

void poweron() { write_cmd(SET_DISP | 0x01); }

void contrast(uint8_t contrast) { write_cmd(SET_CONTRAST); write_cmd(contrast); }

void invert(uint8_t invert) { write_cmd(SET_NORM_INV | (invert & 1)); }


static void init_i2c(int sda)
{
	if( sda % 4 == 0)
		i2c_port = i2c0;
	else
		i2c_port = i2c1;

	// This example will use I2C0 on GPIO4 (SDA) and GPIO5 (SCL)
	i2c_init(i2c_port, 1000 * 1000);
	gpio_set_function(sda, GPIO_FUNC_I2C);
	gpio_set_function(sda+1, GPIO_FUNC_I2C);
	gpio_pull_up(sda);
	gpio_pull_up(sda+1);
}

void reset_addressing(void)
{
	write_cmd(SET_MEM_ADDR); // 0x20
	//write_cmd(0b01); // vertical addressing mode
	write_cmd(0b00); // horizontal addressing mode

	write_cmd(SET_COL_ADDR); // 0x21
	write_cmd(0);
	write_cmd(127);

	write_cmd(SET_PAGE_ADDR); // 0x22
	write_cmd(0);
	write_cmd(pages()-1);

}

void init_display(int h, int sda)
{
	init_i2c(sda);
	height = h;

	static uint8_t cmds[] = {
		SET_DISP | 0x00,  // display off 0x0E | 0x00

		SET_MEM_ADDR, // 0x20
		0x00,  // horizontal

		//# resolution and layout
		SET_DISP_START_LINE | 0x00, // 0x40
		SET_SEG_REMAP | 0x01,  //# column addr 127 mapped to SEG0

		SET_MUX_RATIO, // 0xA8
		(uint8_t)(height - 1),

		SET_COM_OUT_DIR | 0x08,  //# scan from COM[N] to COM0  (0xC0 | val)
		SET_DISP_OFFSET, // 0xD3
		0x00,

		//SET_COM_PIN_CFG, // 0xDA
		//0x02 if self.width > 2 * self.height else 0x12,
		//width > 2*height ? 0x02 : 0x12,
		SET_COM_PIN_CFG, (uint8_t)(height == 32 ? 0x02 : 0x12),

		//# timing and driving scheme
		SET_DISP_CLK_DIV, // 0xD5
		0x80,

		SET_PRECHARGE, // 0xD9
		//0x22 if self.external_vcc else 0xF1,
		//external_vcc ? 0x22 : 0xF1,
		0xF1,

		SET_VCOM_DESEL, // 0xDB
		//0x30,  //# 0.83*Vcc
		0x40, // changed by mcarter

		//# display
		SET_CONTRAST, // 0x81
		0xFF,  //# maximum

		SET_ENTIRE_ON,  //# output follows RAM contents // 0xA4
		SET_NORM_INV,  //# not inverted 0xA6

		SET_CHARGE_PUMP, // 0x8D
		//0x10 if self.external_vcc else 0x14,
		//external_vcc ? 0x10 : 0x14,
		0x14,

		SET_DISP | 0x01
	};

	// write all the commands
	for(int i=0; i<sizeof(cmds); i++)
		write_cmd(cmds[i]);
	clear_scr();
	reset_addressing();
	show_scr();
}


void draw_pixel(int16_t x, int16_t y, int color) 
{
	if(x<0 || x >= width || y<0 || y>= height) return;

	int page = y/8;
	int bit = 1<<(y % 8);
	//int xincr = 8;
	//xincr =	height/8;
	//uint8_t* ptr = scr + x*xincr + page  + 1; 
	//uint8_t* ptr = screen.display[page] + x; 
	//uint8_t* ptr = (uint8_t*) &screen + page*8 + x +1; 
	uint8_t* ptr = scr + page*128 + x + 1;

	switch (color) {
		case 1: // white
			*ptr |= bit;
			break;
		case 0: // black
			*ptr &= ~bit;
			break;
		case -1: //inverse
			*ptr ^= bit;
			break;
	}

}

void drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w,
		int16_t h, uint16_t color) 
{
	int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
	uint8_t byte = 0;

	for (int16_t j = 0; j < h; j++, y++) {
		for (int16_t i = 0; i < w; i++) {
			if (i & 7)
				byte <<= 1;
			else
				byte = bitmap[j * byteWidth + i / 8];
			if (byte & 0x80)
				draw_pixel(x + i, y, color);
		}
	}
}

void draw_letter_at (uint8_t x, uint8_t y, char c)
{
	if(c< ' ' || c>  0x7F) c = '?'; // 0x7F is the DEL key

	int offset = 4 + (c - ' ' )*6;
	for(int col = 0 ; col < 6; col++) {
		uint8_t line =  ssd1306_font6x8[offset+col];
		for(int row =0; row <8; row++) {
			draw_pixel(x+col, y+row, line & 1);
			line >>= 1;
		}
	}

	for(int row = 0; row<8; row++) {
		draw_pixel(x+6, y+row, 0);
		draw_pixel(x+7, y+row, 0);
	}

}


// void draw_letter(char c) { draw_letter_at(0, 0, c); }

static int cursorx = 0, cursory = 0;


void ssd1306_putchar(char c)
{
	if(c == '\n') {
		cursorx = 0;
		cursory += 8;
		return;
	}
	if(cursorx >= 128) {
		cursorx = 0;
		cursory += 8;
	}
	if(cursory >= height) { // scroll up
		memmove(scr, scr + 128, (pages()-1)*128);
		memset(scr + (pages()-1)*128 , 0, 128);
		cursorx = 0;
		cursory = (pages()-1)*8;
	}
	draw_letter_at(cursorx, cursory, c);
	cursorx += 8;
}

/** @brief Print at current cursor position
*/

void ssd1306_print(const char* str)
{
	char c;
	while(c = *str) {
		ssd1306_putchar(c);
		str++;
	}
}

void ssd1306_puts(const char* str)
{
	ssd1306_print(str);
	ssd1306_putchar('\n');
}

void ssd1306_printf(const char *format, ...)
{
	char str[512];
	va_list ap;
	va_start(ap, format);
	vsnprintf(str, sizeof(str), format, ap);
	va_end(ap);
	str[511] = 0;
	ssd1306_print(str);
}


/** @brief Print at a stated cursor position

Position should be given in terms of character positions, not pixel

*/

void ssd1306_print_at(int x, int y, const char* str)
{
        setCursorx(x);
        setCursory(y);
        ssd1306_print(str);
}

void setCursorx(int x)
{
	const int pos = 8;
	cursorx = pos * x;
}


void setCursory(int y)
{
	const int pos = 8;
	cursory = pos * y;
}


/*
    MIT License
    Copyright (c) 2018-2019, Alexey Dynda
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:
    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/
/*
 * ssd1306xled_font6x8 is by Neven Boyanov
 * ssd1306xled_font8x16 is by Neven Boyanov
 *
 * @created: 2014-08-12
 * @author: Neven Boyanov
 *
 * Copyright (c) 2015 Neven Boyanov, Tinusaur Team. All Rights Reserved.
 * Distributed as open source software under MIT License, see LICENSE.txt file.
 * Please, as a favour, retain the link http://tinusaur.org to The Tinusaur Project.
 *
 * Source code available at: https://bitbucket.org/tinusaur/ssd1306xled
 *
 */

/* Source:
 * https://github.com/lexus2k/ssd1306/blob/master/src/ssd1306_fonts.c
 */




/* Standard ASCII 6x8 font */
extern "C" const uint8_t ssd1306_font6x8[]=
{
  0x00, 0x06, 0x08, 0x20,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // sp
  0x00, 0x00, 0x00, 0x2f, 0x00, 0x00, // !
  0x00, 0x00, 0x07, 0x00, 0x07, 0x00, // "
  0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14, // #
  0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12, // $
  0x00, 0x23, 0x13, 0x08, 0x64, 0x62, // %
  0x00, 0x36, 0x49, 0x55, 0x22, 0x50, // &
  0x00, 0x00, 0x05, 0x03, 0x00, 0x00, // '
  0x00, 0x00, 0x1c, 0x22, 0x41, 0x00, // (
  0x00, 0x00, 0x41, 0x22, 0x1c, 0x00, // )
  0x00, 0x14, 0x08, 0x3E, 0x08, 0x14, // *
  0x00, 0x08, 0x08, 0x3E, 0x08, 0x08, // +
  0x00, 0x00, 0x00, 0xA0, 0x60, 0x00, // ,
  0x00, 0x08, 0x08, 0x08, 0x08, 0x08, // -
  0x00, 0x00, 0x60, 0x60, 0x00, 0x00, // .
  0x00, 0x20, 0x10, 0x08, 0x04, 0x02, // /
  0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E, // 0
  0x00, 0x00, 0x42, 0x7F, 0x40, 0x00, // 1
  0x00, 0x42, 0x61, 0x51, 0x49, 0x46, // 2
  0x00, 0x21, 0x41, 0x45, 0x4B, 0x31, // 3
  0x00, 0x18, 0x14, 0x12, 0x7F, 0x10, // 4
  0x00, 0x27, 0x45, 0x45, 0x45, 0x39, // 5
  0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30, // 6
  0x00, 0x01, 0x71, 0x09, 0x05, 0x03, // 7
  0x00, 0x36, 0x49, 0x49, 0x49, 0x36, // 8
  0x00, 0x06, 0x49, 0x49, 0x29, 0x1E, // 9
  0x00, 0x00, 0x36, 0x36, 0x00, 0x00, // :
  0x00, 0x00, 0x56, 0x36, 0x00, 0x00, // ;
  0x00, 0x08, 0x14, 0x22, 0x41, 0x00, // <
  0x00, 0x14, 0x14, 0x14, 0x14, 0x14, // =
  0x00, 0x00, 0x41, 0x22, 0x14, 0x08, // >
  0x00, 0x02, 0x01, 0x51, 0x09, 0x06, // ?
  0x00, 0x32, 0x49, 0x59, 0x51, 0x3E, // @
  0x00, 0x7C, 0x12, 0x11, 0x12, 0x7C, // A
  0x00, 0x7F, 0x49, 0x49, 0x49, 0x36, // B
  0x00, 0x3E, 0x41, 0x41, 0x41, 0x22, // C
  0x00, 0x7F, 0x41, 0x41, 0x22, 0x1C, // D
  0x00, 0x7F, 0x49, 0x49, 0x49, 0x41, // E
  0x00, 0x7F, 0x09, 0x09, 0x09, 0x01, // F
  0x00, 0x3E, 0x41, 0x49, 0x49, 0x7A, // G
  0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F, // H
  0x00, 0x00, 0x41, 0x7F, 0x41, 0x00, // I
  0x00, 0x20, 0x40, 0x41, 0x3F, 0x01, // J
  0x00, 0x7F, 0x08, 0x14, 0x22, 0x41, // K
  0x00, 0x7F, 0x40, 0x40, 0x40, 0x40, // L
  0x00, 0x7F, 0x02, 0x0C, 0x02, 0x7F, // M
  0x00, 0x7F, 0x04, 0x08, 0x10, 0x7F, // N
  0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E, // O
  0x00, 0x7F, 0x09, 0x09, 0x09, 0x06, // P
  0x00, 0x3E, 0x41, 0x51, 0x21, 0x5E, // Q
  0x00, 0x7F, 0x09, 0x19, 0x29, 0x46, // R
  0x00, 0x46, 0x49, 0x49, 0x49, 0x31, // S
  0x00, 0x01, 0x01, 0x7F, 0x01, 0x01, // T
  0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F, // U
  0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F, // V
  0x00, 0x3F, 0x40, 0x38, 0x40, 0x3F, // W
  0x00, 0x63, 0x14, 0x08, 0x14, 0x63, // X
  0x00, 0x07, 0x08, 0x70, 0x08, 0x07, // Y
  0x00, 0x61, 0x51, 0x49, 0x45, 0x43, // Z
  0x00, 0x00, 0x7F, 0x41, 0x41, 0x00, // [
  0x00, 0x55, 0x2A, 0x55, 0x2A, 0x55, // 55
  0x00, 0x00, 0x41, 0x41, 0x7F, 0x00, // ]
  0x00, 0x04, 0x02, 0x01, 0x02, 0x04, // ^
  0x00, 0x40, 0x40, 0x40, 0x40, 0x40, // _
  0x00, 0x00, 0x01, 0x02, 0x04, 0x00, // '
  0x00, 0x20, 0x54, 0x54, 0x54, 0x78, // a
  0x00, 0x7F, 0x48, 0x44, 0x44, 0x38, // b
  0x00, 0x38, 0x44, 0x44, 0x44, 0x20, // c
  0x00, 0x38, 0x44, 0x44, 0x48, 0x7F, // d
  0x00, 0x38, 0x54, 0x54, 0x54, 0x18, // e
  0x00, 0x08, 0x7E, 0x09, 0x01, 0x02, // f
  0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C, // g
  0x00, 0x7F, 0x08, 0x04, 0x04, 0x78, // h
  0x00, 0x00, 0x44, 0x7D, 0x40, 0x00, // i
  0x00, 0x40, 0x80, 0x84, 0x7D, 0x00, // j
  0x00, 0x7F, 0x10, 0x28, 0x44, 0x00, // k
  0x00, 0x00, 0x41, 0x7F, 0x40, 0x00, // l
  0x00, 0x7C, 0x04, 0x18, 0x04, 0x78, // m
  0x00, 0x7C, 0x08, 0x04, 0x04, 0x78, // n
  0x00, 0x38, 0x44, 0x44, 0x44, 0x38, // o
  0x00, 0xFC, 0x24, 0x24, 0x24, 0x18, // p
  0x00, 0x18, 0x24, 0x24, 0x18, 0xFC, // q
  0x00, 0x7C, 0x08, 0x04, 0x04, 0x08, // r
  0x00, 0x48, 0x54, 0x54, 0x54, 0x20, // s
  0x00, 0x04, 0x3F, 0x44, 0x40, 0x20, // t
  0x00, 0x3C, 0x40, 0x40, 0x20, 0x7C, // u
  0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C, // v
  0x00, 0x3C, 0x40, 0x30, 0x40, 0x3C, // w
  0x00, 0x44, 0x28, 0x10, 0x28, 0x44, // x
  0x00, 0x1C, 0xA0, 0xA0, 0xA0, 0x7C, // y
  0x00, 0x44, 0x64, 0x54, 0x4C, 0x44, // z
  0x00, 0x00, 0x08, 0x77, 0x00, 0x00, // {
  0x00, 0x00, 0x00, 0x7F, 0x00, 0x00, // |
  0x00, 0x00, 0x77, 0x08, 0x00, 0x00, // }
  0x00, 0x10, 0x08, 0x10, 0x08, 0x00, // ~
  0x14, 0x14, 0x14, 0x14, 0x14, 0x14, // horiz lines // DEL
  0x00 /* This byte is required for italic type of font */
};
