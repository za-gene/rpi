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

#include <basal.h>
#include <gpio.h>
#include <mbox.h>
#include <uart0.h>

/*
#define PBASE       0x3F000000
#define GPSET1          ((volatile unsigned int*)(PBASE+0x00200020))
#define GPFSEL1         ((volatile unsigned int*)(PBASE+0x00200004))
#define GPPUD           ((volatile unsigned int*)(PBASE+0x00200094))
#define GPPUDCLK0       ((volatile unsigned int*)(PBASE+0x00200098))
*/



/* PL011 UART registers */
#define UART0_DR        *((volatile unsigned int*)(PBASE+0x00201000))
#define UART0_FR        *((volatile unsigned int*)(PBASE+0x00201018))
#define UART0_IBRD      *((volatile unsigned int*)(PBASE+0x00201024))
#define UART0_FBRD      *((volatile unsigned int*)(PBASE+0x00201028))
#define UART0_LCRH      *((volatile unsigned int*)(PBASE+0x0020102C))
#define UART0_CR        *((volatile unsigned int*)(PBASE+0x00201030))
#define UART0_IMSC      *((volatile unsigned int*)(PBASE+0x00201038))
#define UART0_ICR       *((volatile unsigned int*)(PBASE+0x00201044))

/**
 * Set baud rate and characteristics (115200 8N1) and map to GPIO
 */
void uart0_init()
{
	register unsigned int r;

	/* initialize UART */
	UART0_CR = 0;         // turn off UART0

	/* set up clock for consistent divisor values */
	mbox[0] = 9*4;
	mbox[1] = MBOX_REQUEST;
	mbox[2] = MBOX_TAG_SETCLKRATE; // set clock rate
	mbox[3] = 12;
	mbox[4] = 8;
	mbox[5] = 2;           // UART clock
	mbox[6] = 4000000;     // 4Mhz
	mbox[7] = 0;           // clear turbo
	mbox[8] = MBOX_TAG_LAST;
	mbox_call(MBOX_CH_PROP);

  
#if 1
    // TX AND RX pins setup
    gpio_sel(14, ALTFN0);
    gpio_sel(15, ALTFN0);
#else
	// map UART0 to GPIO pins 
	r=GPFSEL1;
	r&=~((7<<12)|(7<<15)); // gpio14, gpio15
	r|=(4<<12)|(4<<15);    // alt0
	GPFSEL1 = r;
	GPPUD = 0;            // enable pins 14 and 15
	r=150; while(r--) { asm volatile("nop"); }
	GPPUDCLK0 = (1<<14)|(1<<15);
	r=150; while(r--) { asm volatile("nop"); }
	GPPUDCLK0 = 0;        // flush GPIO setup
#endif


	UART0_ICR = 0x7FF;    // clear interrupts
	UART0_IBRD = 2;       // 115200 baud
	UART0_FBRD = 0xB;
	UART0_LCRH = 0b11<<5; // 8n1
	UART0_CR = 0x301;     // enable Tx, Rx, FIFO
}

static int _uart0_send(int c)
{
	/* wait until we can send */
	do{asm volatile("nop");}while(UART0_FR&0x20);
	/* write the character to the buffer */
	UART0_DR=c;
	return c;
}

static void uart0_newline(void)
{
	_uart0_send('\r');
	_uart0_send('\n');
}

/**
 * Send a character
 */
static int uart0_send(int c) {
	_uart0_send(c);
	if(c=='\n' || c=='\r') uart0_newline();
	return c;
}

int uart0_putchar(int c)
{
	return uart0_send(c);
}

/**
 * Receive a character
 */
int uart0_getc() {
	int r;
	/* wait until something is in the buffer */
	do{asm volatile("nop");}while(UART0_FR & 0x10);
	/* read it and return */
	r=(int)(UART0_DR);
	/* convert carrige return to newline */
	//return r=='\r'?'\n':r;
	return r;
}

/**
 * Display a string
 */
void uart0_puts(char *s) {
	while(*s) {
		/* convert newline to carrige return + newline */
		//if(*s=='\n') uart0_send('\r');
		uart0_send(*s++);
	}
	uart0_newline();
}

/**
 * Display a binary value in hexadecimal
 */
void uart0_hex(unsigned int d) {
	unsigned int n;
	int c;
	for(c=28;c>=0;c-=4) {
		// get highest tetrad
		n=(d>>c)&0xF;
		// 0-9 => '0'-'9', 10-15 => 'A'-'F'
		n+=n>9?0x37:0x30;
		uart0_send(n);
	}
}

void uart0_init_as_stdio()
{
	uart0_init();
	set_putchar(uart0_send);
	set_getchar(uart0_getc);
}
