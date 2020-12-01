#pragma once

/**
 * RPi 0  : BCM2835. 1 core @ 1GHz. 512MB RAM.
 * RPi B+ : BCM2835
 * RPi 2  : BCM2836 quad core @ 1GHz. 1GB RAM.
 * RPi 3  : BCM2837
 * RPi 3A+, 3B+ : BCM2837B0
 *
 * Datasheets (BCM*) can be found at
 * https://github.com/raspberrypi/documentation/tree/master/hardware/raspberrypi
 *
 *
 * BCM2837 ARM periphral datasheet for Pi 3:
 * https://cs140e.sergio.bz/docs/BCM2837-ARM-Peripherals.pdf
 */

#include <inttypes.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

//u64 __aeabi_uidivmod(u32 value, u32 divisor);
//u32 __aeabi_uidiv(u32 value, u32 divisor);

//#define RPI 3

#define GPU_MEM_BASE  0xC0000000 // L2 cache disabled, depending on Pi
// 0x40000000 on the Pi Zero, Pi Zero W, and the first generation of the Raspberry Pi 

#define REG(addr) (*(volatile u32*)(addr))

#if (RPI == 0) || (RPI == 1)
    #define PBASE 0x20000000 // peripheral memory map base
#elif (RPI == 2) || (RPI == 3)
	#define PBASE 0x3F000000
#elif RPI == 4
	#define PBASE 0xFE000000
#else
	#warning RPI definition not set
#endif

