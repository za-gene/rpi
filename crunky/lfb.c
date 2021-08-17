/*
 * Copyright (C) 2018 bzt (bztsrc@github)
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */


#include <stdio.h>
#include <string.h>
#include <basal.h>
#include <lfb.h>
#include "mbox.h"


/* PC Screen Font as used by Linux Console */
typedef struct {
	unsigned int magic;
	unsigned int version;
	unsigned int headersize;
	unsigned int flags;
	unsigned int numglyph;
	unsigned int bytesperglyph;
	unsigned int height;
	unsigned int width;
	unsigned char glyphs;
} __attribute__((packed)) psf_t;
extern volatile unsigned char _binary_font_psf_start; // fixed width
psf_t *font = 0;

unsigned int width, height, pitch, g_isrgb;   /* dimensions and channel order */
unsigned char *lfb;                         /* raw frame buffer address */
int g_x =0, g_y = 0; // cursor position in terms of pixels
int bytesperline;

/*
unsigned int lfb_pitch() { return pitch; }
unsigned char* lfb_buffer() { return lfb; }
unsigned int lfb_width() { return width; }
unsigned int lfb_height() { return height; }
unsigned int isrgb() { return g_isrgb; }


int lfb_num_chars_in_col () { return lfb_width() / font->width; }
int lfb_num_chars_in_row () { return lfb_height() / font->height; }
int lfb_font_height () { return font->height; }
int lfb_font_width () { return font->width; }
*/

#define pinfo(x) printf(#x " = %d\n", x)

void lfb_print_info(void)
{
	pinfo(width);
	pinfo(height);
	pinfo(pitch);
	pinfo(bytesperline);

	if(font) {
		pinfo(font->bytesperglyph);
		pinfo(font->height);
		pinfo(font->width);
	}
}




#define WIDTH 1920
#define HEIGHT 1080 // 2021-08-17 confirmed correct
void lfb_init()
{
	font = (psf_t*)&_binary_font_psf_start;
	bytesperline=(font->width+7)/8;

	mbox[0] = 35*4;
	mbox[1] = MBOX_REQUEST;

	mbox[2] = 0x48003;  //set phy wh
	mbox[3] = 8;
	mbox[4] = 8;
	mbox[5] = WIDTH;         //FrameBufferInfo.width
	mbox[6] = HEIGHT;          //FrameBufferInfo.height

	mbox[7] = 0x48004;  //set virt wh
	mbox[8] = 8;
	mbox[9] = 8;
	mbox[10] = WIDTH;        //FrameBufferInfo.virtual_width
	mbox[11] = HEIGHT;         //FrameBufferInfo.virtual_height

	mbox[12] = 0x48009; //set virt offset
	mbox[13] = 8;
	mbox[14] = 8;
	mbox[15] = 0;           //FrameBufferInfo.x_offset
	mbox[16] = 0;           //FrameBufferInfo.y.offset

	mbox[17] = 0x48005; //set depth
	mbox[18] = 4;
	mbox[19] = 4;
	mbox[20] = 32;          //FrameBufferInfo.depth

	mbox[21] = 0x48006; //set pixel order
	mbox[22] = 4;
	mbox[23] = 4;
	mbox[24] = 1;           //RGB, not BGR preferably

	mbox[25] = 0x40001; //get framebuffer, gets alignment on request
	mbox[26] = 8;
	mbox[27] = 8;
	mbox[28] = 4096;        //FrameBufferInfo.pointer
	mbox[29] = 0;           //FrameBufferInfo.size

	mbox[30] = 0x40008; //get pitch
	mbox[31] = 4;
	mbox[32] = 4;
	mbox[33] = 0;           //FrameBufferInfo.pitch

	mbox[34] = MBOX_TAG_LAST;

	//this might not return exactly what we asked for, could be
	//the closest supported resolution instead
	if(mbox_call(MBOX_CH_PROP) && mbox[20]==32 && mbox[28]!=0) {
		mbox[28]&=0x3FFFFFFF;   //convert GPU address to ARM address
		width=mbox[5];          //get actual physical width
		height=mbox[6];         //get actual physical height
		pitch=mbox[33];         //get number of bytes per line
		g_isrgb=mbox[24];         //get the actual channel order
		lfb=(void*)((unsigned long)mbox[28]);
	} else {
		//uart0_puts("Unable to set screen resolution to 1024x768x32\n");
	}
}

void lfb_init_as_stdout()
{
	lfb_init();
	set_putchar(fbputchar);
}

/**
 * Display a string using fixed size PSF
 */
void lfb_print(int x, int y, char *s)
{
	g_x = x;
	g_y = y;
	while(*s) fbputchar(*s++);
}



int fbputchar(int  c)
{
	char s[1];
	s[0] = (char) c;
	//psf_t *font = (psf_t*)&_binary_font_psf_start; 
	// get the offset of the glyph. Need to adjust this to support unicode table
	unsigned char *glyph = (unsigned char*)&_binary_font_psf_start +
		font->headersize + (*((unsigned char*)s)<font->numglyph?*s:0)*font->bytesperglyph;
	// calculate the offset on screen 
	int offs = (g_y * pitch) + (g_x * 4);
	// variables
	int i,j, line,mask;
	// handle carriage return
	if(*s == '\r') {
		g_x = 0;
	} else
		// new line
		if(*s == '\n') { 
			fbnewline();
		} else {
			// display a character
			for(j=0;j<font->height;j++){
				// display one row
				line=offs;
				mask=1<<(font->width-1);
				for(i=0;i<font->width;i++){
					// if bit set, we use white color, otherwise black
					*((unsigned int*)(lfb + line))=((int)*glyph) & mask?0xFFFFFF:0;
					mask>>=1;
					line+=4;
				}
				// adjust to next line
				glyph+=bytesperline;
				offs+=pitch;
			}
			g_x += (font->width+1);
		}
	return c;
}


void fbprint(char* str)
{
	while(*str)
		fbputchar(*str++);
}

void fbnewline (void)
{
	//fbprint("\n");
	int fh = font->height;
	g_x = 0; 
	g_y += fh;
#define NROWS 40 // basically a guess for now
	if(g_y < fh*NROWS) return;
	//fbprint("fbnewline called");
	int offset = WIDTH*fh*4;
	int bytes_to_copy = (NROWS-0)*offset;
	//printf("offset %d, bytes_to_copy %d", offset, bytes_to_copy);
	//while(1);


	memcpy(lfb, lfb+ offset, bytes_to_copy);
	memset(lfb + offset + bytes_to_copy, WIDTH*fh*4, 0);

	g_y -= fh;
}

int fbputs(char* str)
{
	fbprint(str);
	fbnewline();
	return 1;
}



