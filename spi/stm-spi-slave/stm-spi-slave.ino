// SPI full-duplex slave example
// STM32 acts as a SPI slave and reads 8 bit data frames over SPI.
// Master also gets a reply from the slave, which is a a simple count (0, 1, 2, 3)
// that is incremented each time a data frame is received.
// Serial output is here for debug

#include <SPI.h>



void setup()
{
  // The clock value is not used
  // SPI1 is selected by default
  // MOSI, MISO, SCK and NSS PINs are set by the library
  SPI.beginTransactionSlave(SPISettings(18000000, MSBFIRST, SPI_MODE0, DATA_SIZE_8BIT));
}

void loop()
{
  static uint8_t i = 0;
  SPI.transfer(i++); // blocking call
}
