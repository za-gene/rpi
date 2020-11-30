#pragma once
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

void wait_us(uint64_t n);
//void wait_micros(int n);
void wait_cycles(unsigned int n);
void wait_msec(unsigned int n);
uint64_t get_system_timer();
void delay_ms(int ms);
void delay_s(int secs);



// Per BCM2837 s14.2  p196 for Pi3
#define TBASE (PBASE + 0xB000)
#define ARM_TIMER_LOD REG(TBASE + 0x400)
#define ARM_TIMER_VAL REG(TBASE + 0x404)
#define ARM_TIMER_CTL REG(TBASE + 0x408)
#define ARM_TIMER_CLI REG(TBASE + 0x40C)
#define ARM_TIMER_RIS REG(TBASE + 0x410)
#define ARM_TIMER_MIS REG(TBASE + 0x414)
#define ARM_TIMER_RLD REG(TBASE + 0x418)
#define ARM_TIMER_DIV REG(TBASE + 0x41C)
#define ARM_TIMER_CNT REG(TBASE + 0x420)
