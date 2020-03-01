// https://gist.github.com/nrdobie/8193350
/*
  Basic code for using Maxim MAX7219/MAX7221 with Arduino.

  Wire the Arduino and the MAX7219/MAX7221 together as follows:

  | Arduino   | MAX7219/MAX7221 | WROOM |
  | --------- | --------------- | ----- |
  | MOSI (11) | DIN (1)         | D23   |
  | SCK (13)  | CLK (13)        | D18   |
  | I/O (7)*  | LOAD/CS (12)    | D5    |

    * - This should match the LOAD_PIN constant defined below.
  
  For the rest of the wiring follow the wiring diagram found in the datasheet.
  
  Datasheet: http://datasheets.maximintegrated.com/en/ds/MAX7219-MAX7221.pdf

  Author:  Nicholas Dobie <nick@nickdobie.com>
  Date:    30 December 2013
  License: WTFPL (http://www.wtfpl.net/)
 */
#include <SPI.h>

// What pin on the Arduino connects to the LOAD/CS pin on the MAX7219/MAX7221
#ifdef ESP32 // includes WROOM
#define LOAD_PIN 5
#else
#define LOAD_PIN 10
#endif

/**
 * Transfers data to a MAX7219/MAX7221 register.
 * 
 * @param address The register to load data into
 * @param value   Value to store in the register
 */
void maxTransfer(uint8_t address, uint8_t value) {

  // Ensure LOAD/CS is LOW
  digitalWrite(LOAD_PIN, LOW);

  // Send the register address
  SPI.transfer(address);

  // Send the value
  SPI.transfer(value);

  // Tell chip to load in data
  digitalWrite(LOAD_PIN, HIGH);
}
  

void init_display() {

  //Serial.begin(9600);
  // Set load pin to output
  pinMode(LOAD_PIN, OUTPUT);

  // Reverse the SPI transfer to send the MSB first  
  SPI.setBitOrder(MSBFIRST);  
  SPI.begin();

  maxTransfer(0x0F, 0x00);
  maxTransfer(0x09, 0xFF); // enable mode B 
  maxTransfer(0x0A, 0x0F); // set intensity (page 9)  
  maxTransfer(0x0B, 0x07); // use all pins
  maxTransfer(0x0C, 0x01); // activate chip
}

void set_display(int8_t arr[4])
{
  //int sec = arr[3];
  uint8_t lo, hi;
  for(uint8_t i = 0; i<4; i++) {
    int8_t n = arr[i];
    if(n == -1) { // blankify
        hi = 0b1111;
        lo = 0b1111;
    } else {
      hi = n %10;
      lo = int(n/10);
    }
    if(i==2) 
      hi |= (1<<7); // decimal point
  
    maxTransfer(7-2*i, hi);
    maxTransfer(8-2*i, lo);
  }
}
/*
void loop() {

    for(uint8_t i =0; i< 8; ++i)
    {
      int r = random(10);
      maxTransfer(8-i, r);
      Serial.print(r, DEC);
      Serial.print(" ");
      delay(1);
    }
    Serial.println();
    Serial.flush();
    delay(5000);
}
*/
