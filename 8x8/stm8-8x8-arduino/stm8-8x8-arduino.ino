/*
 * 2020-10-12 Confirmed working (make sure you use the right pins!)
 */
 
#include "I2C.h"

#define SID 0x70 // Slave ID


static uint8_t  pattern[] = { 
  //B11111111,
  B10000001,
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
  /*
    Wire.beginTransmission(0x70); // address of LED matrix
    Wire.write(2*y);
    Wire.write(bits);
    Wire.endTransmission();
  */
}

void setup() {

  I2C_begin();
  send_cmd(0x20 | 1); // turn on oscillator
  send_cmd(0x81); // display on
  send_cmd(0xE0 | 0); // brightness to dimmest (but you should probably set it)

}

void loop() {
  for (int i = 0; i < 8; ++i) {
    u8 row = pattern[i];
    write_row(i, row);
    row = ((row & 0x01)?0x80:0x00) | (row >> 1); // rotate the row for fun
    pattern[i] = row;
  }
  delay(100);
}
