// attempt to convert arduino library to bare metal

// /home/pi/.arduino15/packages/sduino/hardware/stm8/0.5.0/libraries/I2C

#include "I2C.h"

#define SID 0x70 // Slave ID


static uint8_t  pattern[] =
{ B10000001,
  B01000010,
  B00100100,
  B00010000,
  B00001000,
  B00100100,
  B01000010,
  B10000001
};

void send_cmd(u8 cmd) {
  I2C_write(SID, cmd);
}

void send2(u8 cmd, u8 val) {
  char s[2];
  s[0] = val;
  s[1] = 0;
  I2C_write_s(SID, cmd, s);
}

void write_row(uint8_t y, uint8_t xs) {
  // the row of xs beed to be quirkily transformed
  uint8_t bits = 0;
  for (int x = 0; x < 7; ++x) {
    bits <<= 1;
    bits |= (xs & 1);
    xs  >>= 1;
  }
  bits |= (xs << 7);

  // send x layout to device
  send2(2 * y, bits);

}


#define REG(addr) *(volatile u8*)(addr)

#define I2C_CR1   REG(0x5210)
#define I2C_CR2   REG(0x5211)
#define I2C_FREQR   REG(0x5212)
#define I2C_OARL  REG(0x5213)
#define I2C_OARH  REG(0x5214)
#define I2C_DR    REG(0x5216)
#define I2C_SR1   REG(0x5217)

#define     I2C_OARH_ADDMODE (1<<7)               //  7 bit address mode.
#define    I2C_OARH_ADDCONF (1<<6)               //  Docs say this must always be 1.


//#define I2C_FREQR
#define I2C_CCRH_F_S
#define I2C_CCRL REG(0x521B)
#define I2C_CCRH REG(0x521C)
#define I2C_TRISER REG(0x521D)

#define I2C_CR1_PE (1<<0)

#define I2C_CR2_START (1<<0)
#define I2C_CR2_STOP (1<<1)
#define I2C_CR2_ACK (1<<2)

#define I2C_SR1_SB (1<<0)
#define I2C_SR1_RXNE (1<<6)
#define I2C_SR1_TXE (1<<7)

//#define I2C_CR2_ACK = (1<<2)
//#define I2C_CR2_START = 1;


void I2C_Init_copy() {

  uint32_t OutputClockFrequencyHz = I2C_MAX_STANDARD_FREQ;



  /*------------------------- I2C FREQ Configuration ------------------------*/
  uint8_t InputClockFrequencyMHz = 16;
  I2C_FREQR = InputClockFrequencyMHz;
  I2C_TRISER = InputClockFrequencyMHz + 1; // max rise time

  // set clock control frequency registers
  uint16_t speed = (uint16_t)((InputClockFrequencyMHz * 1000000) / (OutputClockFrequencyHz/2));
  if (speed < (uint16_t)0x0004) speed = (uint16_t)0x0004; // must be at least 4
  I2C_CCRL = (uint8_t)speed;
  I2C_CCRH = (uint8_t)(speed >> 8);

  I2C_CR1 |= I2C_CR1_PE; // enable I2C

  /* Configure I2C acknowledgement */
  I2C_Ack_TypeDef Ack = 0;
  I2C_AcknowledgeConfig(Ack);

}



void setup() {

  //I2C_begin();
  //InitialiseI2C();
  I2C_Init_copy();
  send_cmd(0x20 | 1); // turn on oscillator
  send_cmd(0x81); // display on
  send_cmd(0xE0 | 0); // brightness to dimmest (but you should probably set it)

  pattern[0] = 0b11111111;

}

void loop() {
  for (int i = 0; i < 8; ++i) {
    u8 row = pattern[i];
    write_row(i, row);
    row = ((row & 0x01) ? 0x80 : 0x00) | (row >> 1); // rotate the row for fun
    pattern[i] = row;
  }
  delay(100);
}
