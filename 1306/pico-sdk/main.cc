#include <stdint.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"


//# MicroPython SSD1306 OLED driver, I2C and SPI interfaces

//from micropython import const
//import framebuf


#define SET_CONTRAST 0x81
#define SET_ENTIRE_ON 0xA4
#define SET_NORM_INV 0xA6
#define SET_DISP 0xAE
#define SET_MEM_ADDR 0x20
#define SET_COL_ADDR 0x21
#define SET_PAGE_ADDR 0x22
#define SET_DISP_START_LINE 0x40
#define SET_SEG_REMAP 0xA0
#define SET_MUX_RATIO 0xA8
#define SET_COM_OUT_DIR 0xC0
#define SET_DISP_OFFSET 0xD3
#define SET_COM_PIN_CFG 0xDA
#define SET_DISP_CLK_DIV 0xD5
#define SET_PRECHARGE 0xD9
#define SET_VCOM_DESEL 0xDB
#define SET_CHARGE_PUMP 0x8D

typedef uint8_t u8;

#define I2C_PORT i2c0
#define SID 0x3C // I2C 1306 slave ID ... for 64 height display. 32 height is 0x3D

const u8 height = 128;
const u8 width = 64;
const int pages = height / 8;
const bool external_vcc = false;


u8 scr[height*width/8];

void write_cmd(u8 cmd);

void fill_scr(u8 v)
{
	memset(scr, v, sizeof(scr));
}



/* Original Python
   def show(self):
   x0 = 0
   x1 = self.width - 1
   if self.width == 64:
# displays with width of 64 pixels are shifted by 32
x0 += 32
x1 += 32
self.write_cmd(SET_COL_ADDR)
self.write_cmd(x0)
self.write_cmd(x1)
self.write_cmd(SET_PAGE_ADDR)
self.write_cmd(0)
self.write_cmd(self.pages - 1)
self.write_data(self.buffer)
*/

void show_scr()
{
	int x0 = 0;
	int x1 = width -1;
	if(width == 64) { x0 += 32; x1 += 32; } // displays of width 64 are shifted by 32
	write_cmd(SET_COL_ADDR);
	write_cmd(x0);
	write_cmd(x1);
	write_cmd(SET_PAGE_ADDR);
	write_cmd(0);
	write_cmd(pages - 1);

	// write screen data itself
	// very inefficient
	u8 buf[sizeof(scr)+1];
	buf[0] = 0x40;
	memcpy(buf+1, scr, sizeof(scr));
	i2c_write_blocking(I2C_PORT, SID, buf, sizeof(buf), false);
}


void send2(u8 v1, u8 v2)
{
	u8 buf[2];
	buf[0] = v1;
	buf[1] = v2;
	i2c_write_blocking(I2C_PORT, SID, buf, 2, false);
}

void write_cmd(u8 cmd) { send2(0x80, cmd); }


/* Original Python code:
   def write_cmd(self, cmd):
   self.temp[0] = 0x80  # Co=1, D/C#=0
   self.temp[1] = cmd
   self.i2c.writeto(self.addr, self.temp)
   */
void write_cmds(u8* cmds, int n)
{
	for(int i=0; i<n; i++)
		write_cmd(cmds[i]);
}

void poweroff() { write_cmd(SET_DISP | 0x00); }

void poweron() { write_cmd(SET_DISP | 0x01); }

void contrast(u8 contrast) { write_cmd(SET_CONTRAST); write_cmd(contrast); }

void invert(u8 invert) { write_cmd(SET_NORM_INV | (invert & 1)); }


void init_display()
{
	u8 cmds[] = {
		SET_DISP | 0x00,  //# off
		//# address setting
		SET_MEM_ADDR,
		0x00,  //# horizontal
		//# resolution and layout
		SET_DISP_START_LINE | 0x00,
		SET_SEG_REMAP | 0x01,  //# column addr 127 mapped to SEG0
		SET_MUX_RATIO,
		height - 1,
		SET_COM_OUT_DIR | 0x08,  //# scan from COM[N] to COM0
		SET_DISP_OFFSET,
		0x00,
		SET_COM_PIN_CFG,
		//0x02 if self.width > 2 * self.height else 0x12,
		width > 2*height ? 0x02 : 0x12,
		//# timing and driving scheme
		SET_DISP_CLK_DIV,
		0x80,
		SET_PRECHARGE,
		//0x22 if self.external_vcc else 0xF1,
		external_vcc ? 0x22 : 0xF1,
		SET_VCOM_DESEL,
		0x30,  //# 0.83*Vcc
		//# display
		SET_CONTRAST,
		0xFF,  //# maximum
		SET_ENTIRE_ON,  //# output follows RAM contents
		SET_NORM_INV,  //# not inverted
		//# charge pump
		SET_CHARGE_PUMP,
		//0x10 if self.external_vcc else 0x14,
		external_vcc ? 0x10 : 0x14,
		SET_DISP | 0x01
	};
	write_cmds(cmds, sizeof(cmds));
	fill_scr(0);
	show_scr();
}

/*
# Subclassing FrameBuffer provides support for graphics primitives
# http://docs.micropython.org/en/latest/pyboard/library/framebuf.html
class SSD1306(framebuf.FrameBuffer):
def __init__(self, width, height, external_vcc):
self.width = width
self.height = height
self.external_vcc = external_vcc
self.pages = self.height // 8
self.buffer = bytearray(self.pages * self.width)
super().__init__(self.buffer, self.width, self.height, framebuf.MONO_VLSB)
self.init_display()


class SSD1306_I2C(SSD1306):
def __init__(self, width, height, i2c, addr=0x3C, external_vcc=False):
self.i2c = i2c
self.addr = addr
self.temp = bytearray(2)
self.write_list = [b"\x40", None]  # Co=0, D/C#=1
super().__init__(width, height, external_vcc)

def write_data(self, buf):
self.write_list[1] = buf
self.i2c.writevto(self.addr, self.write_list)
*/

void init_i2c()
{
	// This example will use I2C0 on GPIO4 (SDA) and GPIO5 (SCL)
	i2c_init(I2C_PORT, 100 * 1000);
	gpio_set_function(4, GPIO_FUNC_I2C);
	gpio_set_function(5, GPIO_FUNC_I2C);
	gpio_pull_up(4);
	gpio_pull_up(5);
}

void draw_pixel(int16_t x, int16_t y, int color) 
{
	if(x<0 || x >= width || y<0 || y>= height) return;

	int offset = x +(y/8)*width;
	switch (color) {
		case 1: // white
			scr[offset] |= (1 << (y & 7));
			break;
		case 0: // black
			scr[offset] &= ~(1 << (y & 7));
			break;
		case -1: //inverse
			scr[offset] ^= (1 << (y & 7));
			break;
	}

}


u8 letterH[] = {
	0b10000010,
	0b10000010,
	0b10000010,
	0b11111110,
	0b10000010,
	0b10000010,
	0b10000010,
	0b00000000
};

u8 letterP[] = {
	0b11111100,
	0b10000010,
	0b10000010,
	0b11111100,
	0b10000000,
	0b10000000,
	0b10000000,
	0b00000000
};

void draw_letter(u8 letter[]) {
	//clear1306();
	for (int i = 0; i < 8; i++) {
		u8 row = letter[i];
		for (int j = 0; j < 8; j++) {
			u8 on = (row & 1 << 7) ? 1 : 0;
			draw_pixel(j, i, on);
			row <<= 1;
		}
	}
	//display1306();
	//delayish(2000);
}

void once()
{
	init_i2c();
	init_display();
	fill_scr(0);
	draw_letter(letterH);
#if 0
	for(int x = 0; x< 10; x++) 
	for(int y = 0; y< 20; y++) 
		draw_pixel(x,y, 1);
#endif
	show_scr();
	sleep_ms(500);
}

int main()
{
	for(;;) once();
	return 0;
}
