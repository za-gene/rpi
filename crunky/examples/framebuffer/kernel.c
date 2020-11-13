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

#include <gpio.h>
#include <mini_uart.h>

#include "homer.h"

unsigned int width, height, pitch, isrgb;   /* dimensions and channel order */
unsigned char *lfb;                         /* raw frame buffer address */

/* Make sure the property tag buffer is aligned to a 16-byte boundary because
 *    we only have 28-bits available in the property interface protocol to pass
 *       the address of the buffer to the VC. */
// ... according to https://www.valvers.com/open-software/raspberry-pi/bare-metal-programming-in-c-part-5/
static int mbox[8192] __attribute__((aligned(16)));



#define W 1920
#define  H 1020

/**
 * Set screen resolution to 1024x768
 * My screen resolution is 1920x1080@60Hz
 *
 */
void lfb_init()
{
	mbox[0] = 35*4;
	mbox[1] = MBOX_REQUEST;

	mbox[2] = 0x48003;  //set phy wh
	mbox[3] = 8;
	mbox[4] = 8;
	mbox[5] = W;         //FrameBufferInfo.width
	mbox[6] = H;          //FrameBufferInfo.height

	mbox[7] = 0x48004;  //set virt wh
	mbox[8] = 8;
	mbox[9] = 8;
	mbox[10] = W;        //FrameBufferInfo.virtual_width
	mbox[11] = H;         //FrameBufferInfo.virtual_height

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
	if(mbox_call(MBOX_CH_PROP, mbox) && mbox[20]==32 && mbox[28]!=0) {
		mbox[28]&=0x3FFFFFFF;   //convert GPU address to ARM address
		width=mbox[5];          //get actual physical width
		height=mbox[6];         //get actual physical height
		pitch=mbox[33];         //get number of bytes per line
		isrgb=mbox[24];         //get the actual channel order
		lfb=(void*)((unsigned long)mbox[28]);
	} else {
		uart_puts("Unable to set screen resolution\r\n");
	}
}

/**
 * Show a picture
 */
void lfb_showpicture()
{
	int x,y;
	unsigned char *ptr=lfb;
	char *data=homer_data, pixel[4];

	ptr += (height-homer_height)/2*pitch + (width-homer_width)*2;
	for(y=0;y<homer_height;y++) {
		for(x=0;x<homer_width;x++) {
			HEADER_PIXEL(data, pixel);
			// the image is in RGB. So if we have an RGB framebuffer, we can copy the pixels
			// directly, but for BGR we must swap R (pixel[0]) and B (pixel[2]) channels.
			*((unsigned int*)ptr)=isrgb ? *((unsigned int *)&pixel) : (unsigned int)(pixel[0]<<16 | pixel[1]<<8 | pixel[2]);
			ptr+=4;
		}
		ptr+=pitch-homer_width*4;
	}
}
void kernel_main()
{
	uart_init(9600);
	uart_puts("Homer demo\r\n");
	lfb_init();
	uart_puts("Init finished\r\n");
	lfb_showpicture();
	uart_puts("Show finished\r\n");

	while(1);
}
