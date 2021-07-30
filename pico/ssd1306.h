#pragma once

#include <stdint.h>

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
#define SET_COM_PIN_CFG 0xDA	// s10.1.18 page 40
#define SET_DISP_CLK_DIV 0xD5
#define SET_PRECHARGE 0xD9
#define SET_VCOM_DESEL 0xDB
#define SET_CHARGE_PUMP 0x8D

void init_display(int h, int sda);
void setCursorx(int x);
void setCursory(int y);
void ssd1306_print(const char* str);
void ssd1306_print_at(int x, int y, const char* str);
void draw_letter_at(uint8_t x, uint8_t y, char c);
void drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w,
		int16_t h, uint16_t color);
void show_scr(void);
void fill_scr(uint8_t v);
void clear_scr(void);
void ssd1306_putchar(char c);
void ssd1306_send_data(uint8_t* data, int nbytes);
void ssd1306_write_cmd(uint8_t cmd);
void ssd1306_display_cell(void);
