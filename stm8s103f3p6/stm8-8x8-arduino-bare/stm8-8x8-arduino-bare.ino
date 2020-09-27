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

#define I2C_CR1   REG(0x5200)
#define I2C_CR2   REG(0x5201)
#define I2C_FREQR   REG(0x5212)
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
  uint16_t OwnAddress = 0xA0;
  I2C_DutyCycle_TypeDef I2C_DutyCycle = 0;
  I2C_Ack_TypeDef Ack = 0;
  I2C_AddMode_TypeDef AddMode = 0;
  uint8_t InputClockFrequencyMHz = 16;

  uint16_t result = 0x0004;
  uint16_t tmpval = 0;
  uint8_t tmpccrh = 0;

 

  /*------------------------- I2C FREQ Configuration ------------------------*/
  /* Clear frequency bits */
  I2C->FREQR &= (uint8_t)(~I2C_FREQR_FREQ);
  /* Write new value */
  I2C->FREQR |= InputClockFrequencyMHz;

  /*--------------------------- I2C CCR Configuration ------------------------*/
  /* Disable I2C to configure TRISER */
  I2C->CR1 &= (uint8_t)(~I2C_CR1_PE);

  /* Clear CCRH & CCRL */
  I2C->CCRH &= (uint8_t)(~(I2C_CCRH_FS | I2C_CCRH_DUTY | I2C_CCRH_CCR));
  I2C->CCRL &= (uint8_t)(~I2C_CCRL_CCR);

  /* Detect Fast or Standard mode depending on the Output clock frequency selected */
  if (OutputClockFrequencyHz > I2C_MAX_STANDARD_FREQ) /* FAST MODE */
  {
    /* Set F/S bit for fast mode */
    tmpccrh = I2C_CCRH_FS;

    if (I2C_DutyCycle == I2C_DUTYCYCLE_2)
    {
      /* Fast mode speed calculate: Tlow/Thigh = 2 */
      result = (uint16_t) ((InputClockFrequencyMHz * 1000000) / (OutputClockFrequencyHz * 3));
    }
    else /* I2C_DUTYCYCLE_16_9 */
    {
      /* Fast mode speed calculate: Tlow/Thigh = 16/9 */
      result = (uint16_t) ((InputClockFrequencyMHz * 1000000) / (OutputClockFrequencyHz * 25));
      /* Set DUTY bit */
      tmpccrh |= I2C_CCRH_DUTY;
    }

    /* Verify and correct CCR value if below minimum value */
    if (result < (uint16_t)0x01)
    {
      /* Set the minimum allowed value */
      result = (uint16_t)0x0001;
    }

    /* Set Maximum Rise Time: 300ns max in Fast Mode
      = [300ns/(1/InputClockFrequencyMHz.10e6)]+1
      = [(InputClockFrequencyMHz * 3)/10]+1 */
    tmpval = ((InputClockFrequencyMHz * 3) / 10) + 1;
    I2C->TRISER = (uint8_t)tmpval;

  }
  else /* STANDARD MODE */
  {

    /* Calculate standard mode speed */
    result = (uint16_t)((InputClockFrequencyMHz * 1000000) / (OutputClockFrequencyHz << (uint8_t)1));

    /* Verify and correct CCR value if below minimum value */
    if (result < (uint16_t)0x0004)
    {
      /* Set the minimum allowed value */
      result = (uint16_t)0x0004;
    }

    /* Set Maximum Rise Time: 1000ns max in Standard Mode
      = [1000ns/(1/InputClockFrequencyMHz.10e6)]+1
      = InputClockFrequencyMHz+1 */
    I2C->TRISER = (uint8_t)(InputClockFrequencyMHz + (uint8_t)1);

  }

  /* Write CCR with new calculated value */
  I2C->CCRL = (uint8_t)result;
  I2C->CCRH = (uint8_t)((uint8_t)((uint8_t)(result >> 8) & I2C_CCRH_CCR) | tmpccrh);

  /* Enable I2C */
  I2C->CR1 |= I2C_CR1_PE;

  /* Configure I2C acknowledgement */
  I2C_AcknowledgeConfig(Ack);

  /*--------------------------- I2C OAR Configuration ------------------------*/
  I2C->OARL = (uint8_t)(OwnAddress);
  I2C->OARH = (uint8_t)((uint8_t)(AddMode | I2C_OARH_ADDCONF) |
                        (uint8_t)((OwnAddress & (uint16_t)0x0300) >> (uint8_t)7));
}



void setup() {

  //I2C_begin();
  //InitialiseI2C();
  I2C_Init_copy();
  send_cmd(0x20 | 1); // turn on oscillator
  send_cmd(0x81); // display on
  send_cmd(0xE0 | 0); // brightness to dimmest (but you should probably set it)

  //pattern[0] = 0b11111111;

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
