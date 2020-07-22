// https://gist.github.com/nrdobie/8193350
/*
  Basic code for using Maxim MAX7219/MAX7221 with Arduino.

  Wire the Arduino and the MAX7219/MAX7221 together as follows:

  | Arduino   | MAX7219/MAX7221 | WROOM |
  | --------- | --------------- | ----- |
  | MOSI (11) | DIN (1)         | D23   |
  | SCK (13)  | CLK (13)        | D18   |
  | I/O (7)*  | LOAD/CS (12)    | D5    |

      - This should match the LOAD_PIN constant defined below.

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
   Transfers data to a MAX7219/MAX7221 register.

   @param address The register to load data into
   @param value   Value to store in the register
*/
void transfer_7219(uint8_t address, uint8_t value) {
  digitalWrite(LOAD_PIN, LOW); // CS
  SPI.transfer(address);
  SPI.transfer(value);
  digitalWrite(LOAD_PIN, HIGH);
}

void init_7219() {
  pinMode(LOAD_PIN, OUTPUT);
  SPI.setBitOrder(MSBFIRST);
  SPI.begin();
  transfer_7219(0x0F, 0x00);
  transfer_7219(0x09, 0xFF); // Enable mode B
  transfer_7219(0x0A, 0x0F); // set intensity (page 9)
  transfer_7219(0x0B, 0x07); // use all pins
  transfer_7219(0x0C, 0x01); // Turn on chip
}

void setup() {
  Serial.begin(9600);
  init_7219();
}

void loop() {
  for (uint8_t i = 0; i < 8; ++i)
  {
    int r = random(10);
    transfer_7219(8 - i, r);
    Serial.print(r, DEC);
    Serial.print(" ");
    delay(1);
  }
  Serial.println();
  Serial.flush();
  delay(5000);
}
