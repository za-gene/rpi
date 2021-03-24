///////////////////////////////////////////////////////////
// MAX7219

#include <zeroseg.h>

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


// MAX7219 end
