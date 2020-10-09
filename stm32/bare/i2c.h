#pragma once
#include "blue.h"

typedef struct {
	__IO u32 CR1; //0x00
	__IO u32 CR2; // 0x04
	__IO u32 OAR1; //0x08
	__IO u32 OAR2; //0x0C
	__IO u32 DR; //0x10
	__IO u32 SR1; //0x14
	__IO u32 SR2; //0x18
	__IO u32 CCR; // 0x1C
	__IO u32 TRISE; // 0x20
} I2C_t;

#define I2C1 ((I2C_t*) (0x40005400))
#define I2C2 ((I2C_t*) (0x40005800))


#define I2C_CR1_PE (1<<0)

#define I2C_CR1_STOP (1<<9)
#define I2C_CR1_ACK (1<<10)
#define I2C_CR1_START (1<<8)
#define I2C_CR1_SWRST (1<<15)

#define I2C_CR2_LAST (1<<12)
#define I2C_CR2_DMAEN (1<<11)

#define I2C_SR1_SB (1<<0)
#define I2C_SR1_RXNE (1<<6)
#define I2C_SR1_TXE  (1<<7)
#define I2C_SR1_ADDR (1<<1)

void init_i2c();
void i2c_read_dma(u8 sid, u8* buffer, u32 len);
void i2c_read(u8 sid, u8* buffer, u32 len);
void write_i2c(u8 sid, u8* buffer, u32 len);
