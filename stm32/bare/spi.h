#pragma once

#include "blue.h"

struct SPI_t; // defined later

#define SPI1 ((SPI_t*) 0x40013000)
#define SPI3 ((SPI_t*) 0x40003C00)
#define SPI2 ((SPI_t*) 0x40003800)

// section 25.5 page 742
typedef struct {
	__IO u32 CR1;     // 0x00
	__IO u32 CR2;     // 0x04
	__IO u32 SR;      // 0x08
	__IO u32 DR;      // 0x0C
	__IO u32 CRCPR;   // 0x10
	__IO u32 RXCRCR;  // 0x14
	__IO u32 TXCRCR;  // 0x18
	__IO u32 I2SCFGR; // 0x1C
	__IO u32 I2SPR;   // 0x20
} SPI_t;

#define SPI_CR1_MSTR (1<<2)
#define SPI_CR1_SPE (1<<6)
#define SPI_CR1_SSI (1<<8)
#define SPI_CR1_SSM (1<<9)
#define SPI_CR1_BIDIMODE (1<<15)
#define SPI_CR1_BIDIOE (1<<14)

#define SPI_CR2_SSOE (1<<2)

#define SPI_SR_RXNE (1<<0)
#define SPI_SR_TXE (1<<1)
#define SPI_SR_BSY (1<<7)

void init_spi();
u8 spi_transfer(u8 data);
