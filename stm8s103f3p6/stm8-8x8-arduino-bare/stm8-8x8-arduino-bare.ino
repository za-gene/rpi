// attempt to convert arduino library to bare metal

// /home/pi/.arduino15/packages/sduino/hardware/stm8/0.5.0/libraries/I2C
// /home/pi/.arduino15/packages/sduino/hardware/stm8/0.5.0/STM8S_StdPeriph_Driver/src

#include "I2C.h"

#define SID 0x70 // Slave ID

#define REG(addr) *(volatile u8*)(addr)

#define I2C_CR1   REG(0x5210)
#define I2C_CR2   REG(0x5211)
#define I2C_FREQR   REG(0x5212)
#define I2C_OARL  REG(0x5213)
#define I2C_OARH  REG(0x5214)
#define I2C_DR    REG(0x5216)
#define I2C_SR1   REG(0x5217)

//#define     I2C_OARH_ADDMODE (1<<7)               //  7 bit address mode.
//#define    I2C_OARH_ADDCONF (1<<6)               //  Docs say this must always be 1.


//#define I2C_FREQR
#define I2C_CCRH_F_S
#define I2C_CCRL REG(0x521B)
#define I2C_CCRH REG(0x521C)
#define I2C_TRISER REG(0x521D)

//#define I2C_CR1_PE (1<<0)

//#define I2C_CR2_START (1<<0)
//#define I2C_CR2_STOP (1<<1)
//#define I2C_CR2_ACK (1<<2)

//#define I2C_SR1_SB (1<<0)
//#define I2C_SR1_RXNE (1<<6)
//#define I2C_SR1_TXE (1<<7)

//#define I2C_CR2_ACK = (1<<2)
//#define I2C_CR2_START = 1;

static uint16_t timeOutDelay;
//uint8_t returnStatus;

#define SLA_W(address)  (address << 1)

static void lockUp(void)
{
#if 0
  TWCR = 0;   //releases SDA and SCL lines to high impedance
  TWCR = _BV(TWEN) | _BV(TWEA); //reinitialize TWI 
#endif
  //FIXME: this needs to be checked in detail. CR1 might be involved
  // don't do a full software reset here. That would require a full
  // re-initialization before the next transfer could happen.
  I2C->CR2 = 0;
}


#define TIMEOUT_WAIT_FOR_ZERO(CONDITION,ERROR) \
  while (CONDITION)   /* wait while the condition is still true */ \
  { \
          if(!timeOutDelay){continue;} \
          if((((uint16_t)millis()) - startingTime) >= timeOutDelay) \
          { \
            lockUp(); \
            return(ERROR);    /* return the appropriate error code */ \
          } \
        }

#define TIMEOUT_WAIT_FOR_ONE(CONDITION,ERROR) TIMEOUT_WAIT_FOR_ZERO(!(CONDITION), ERROR)

static void stop(void)
{
  while(!((I2C->SR1 & (I2C_SR1_TXE | I2C_SR1_BTF)) == (I2C_SR1_TXE | I2C_SR1_BTF)));

  I2C->CR2 |= I2C_CR2_STOP;
  while(I2C->CR2 & I2C_CR2_STOP);
}

void sendByte(uint8_t i2cData)
{
  while(!(I2C->SR1 & I2C_SR1_TXE));
  I2C->DR = i2cData;
}

static uint8_t sendAddress(uint8_t i2cAddress, uint8_t mode)
{
  uint16_t startingTime = millis();

  /* do not wait for BUSY==0 as this would block for repeated start */
//  TIMEOUT_WAIT_FOR_ZERO((I2C->SR3 & I2C_SR3_BUSY), 1);

  I2C->CR2 |= I2C_CR2_ACK;  // set ACK
  /* send start sequence */
  I2C->CR2 |= I2C_CR2_START;  // send start sequence

  /* Test on EV5 and clear it (SB flag) */
  TIMEOUT_WAIT_FOR_ONE(I2C->SR1 & I2C_SR1_SB, 1);

  /* Send the Address + Direction */
  I2C->DR = i2cAddress; // I2C_Send7bitAddress()

  /* Test on EV6, but don't clear it yet (ADDR flag) */
  // error code 2: no ACK received on address transmission
  TIMEOUT_WAIT_FOR_ONE(I2C->SR1 & I2C_SR1_ADDR, 2);

  if (mode == 1) {
    I2C->CR2 &= ~I2C_CR2_ACK; // clear ACK
    BEGIN_CRITICAL      // disable interrupts
    (void) I2C->SR3;    // read SR3 to clear ADDR event bit
    I2C->CR2 |= I2C_CR2_STOP; // send STOP soon
    END_CRITICAL      // enable interrupts
  } else if (mode == 2) {
    I2C->CR2 |= I2C_CR2_POS;  // set POS
    BEGIN_CRITICAL      // disable interrupts
    (void) I2C->SR3;    // read SR3 to clear ADDR event bit
    I2C->CR2 &= ~I2C_CR2_ACK; // clear ACK
    END_CRITICAL      // enable interrupts
  } else {
    (void)I2C->SR3;   // read SR3 to clear ADDR event bit
  }

  return 0;
}

#define MODE_WRITE 4




  
void my_I2C_write_sn(uint8_t address, uint8_t registerAddress, uint8_t * data,
         uint8_t numberBytes)
{
  sendAddress(SLA_W(address),MODE_WRITE);
  sendByte(registerAddress);
// only this part differs for the variants of I2C_write()
  while (numberBytes--) {
    sendByte(*data++);
  }
//
  stop();
 }

void my_I2C_write(uint8_t address, uint8_t registerAddress)
{
  my_I2C_write_sn(address, registerAddress, NULL, 0);
}


void send_cmd(u8 cmd) {
  my_I2C_write(SID, cmd);
}

void send2(u8 cmd, u8 val) {
  char s[2];
  s[0] = val;
  s[1] = 0;
  my_I2C_write_sn(SID, cmd, s, 1);
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



void I2C_Init_copy() {
  uint32_t OutputClockFrequencyHz = I2C_MAX_STANDARD_FREQ;
  uint8_t InputClockFrequencyMHz = 16;
  I2C_FREQR = InputClockFrequencyMHz;
  I2C_TRISER = InputClockFrequencyMHz + 1; // max rise time

  // set clock control frequency registers
  uint16_t speed = (uint16_t)((InputClockFrequencyMHz * 1000000) / (OutputClockFrequencyHz/2));
  if (speed < (uint16_t)0x0004) speed = (uint16_t)0x0004; // must be at least 4
  I2C_CCRL = (uint8_t)speed;
  I2C_CCRH = (uint8_t)(speed >> 8);

  I2C_CR1 |= I2C_CR1_PE; // enable I2C 
}


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
