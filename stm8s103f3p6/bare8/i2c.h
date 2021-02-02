#pragma once

#include <stm8.h>

#define I2C_CR1   REG(0x5210)
#define I2C_CR2   REG(0x5211)
#define I2C_FREQR   REG(0x5212)
#define I2C_OARL  REG(0x5213)
#define I2C_OARH  REG(0x5214)
#define I2C_DR    REG(0x5216)
#define I2C_SR1   REG(0x5217)
#define I2C_SR2   REG(0x5218)
#define I2C_SR3   REG(0x5219)
#define I2C_CCRL REG(0x521B)
#define I2C_CCRH REG(0x521C)
#define I2C_TRISER REG(0x521D)

#define I2C_OARH_ADDMODE (1<<7) // hmmm, goes to 10-bit addr

#define I2C_MAX_STANDARD_FREQ ((uint32_t)100000)

//#undef I2C_CR1_PE

#define I2C_CR1_PE (1<<0)

//#undef I2C_CR2_START 
//#undef I2C_CR2_STOP 
//#undef I2C_CR2_ACK


#define I2C_CR2_START (1<<0)
#define I2C_CR2_STOP (1<<1)
#define I2C_CR2_ACK (1<<2)

//#undef I2C_SR1_SB
//#undef I2C_SR1_RXNE
//#undef I2C_SR1_TXE


#define I2C_SR1_SB (1<<0)
#define I2C_SR1_ADDR (1<<1)
#define I2C_SR1_BTF (1<<2)
#define I2C_SR1_RXNE (1<<6)
#define I2C_SR1_TXE (1<<7)

#define IC2_SR2_AF (1<<2)
#define IC2_SR2_BERR (1<<0)

#define I2C_SR3_MSL (1<<0)

