

/* The ESP32 has four SPi buses, however as of right now only two of
 * them are available to use, HSPI and VSPI. Simply using the SPI API 
 * as illustrated in Arduino examples will use VSPI, leaving HSPI unused.
 * 
 * However if we simply intialise two instance of the SPI class for both
 * of these buses both can be used. However when just using these the Arduino
 * way only will actually be outputting at a time.
 * 
 * Logic analyser capture is in the same folder as this example as
 * "multiple_bus_output.png"
 * 
 * created 30/04/2018 by Alistair Symonds
 */
#include <SPI.h>

static const int spiClk = 200000; // 200kHz

//uninitalised pointers to SPI objects
//SPIClass * vspi = NULL;
//SPIClass * hspi = NULL;
SPIClass vspi(VSPI);

void setup() {
  //initialise two instances of the SPIClass attached to VSPI and HSPI respectively
  //vspi = new SPIClass(VSPI);
  //hspi = new SPIClass(HSPI);
  Serial.begin(115200);
  Serial.println("Hello from esp32 spi master");
  
  //clock miso mosi ss

  //initialise vspi with default pins
  //SCLK = 18, MISO = 19, MOSI = 23, SS = 5
  //vspi->begin();
  vspi.begin();

  pinMode(5, OUTPUT); //VSPI SS

}

// the loop function runs over and over again until power down or reset
void loop() {
  //use the SPI buses
  vspiCommand();
  //hspiCommand();
  delay(1000);
}

void vspiCommand() {
  byte data = 0b01010101; // junk data to illustrate usage

  //use it as you would the regular arduino SPI API
  vspi.beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
  digitalWrite(5, LOW); //pull SS slow to prep other end for transfer
  int res = vspi.transfer(data);  
  digitalWrite(5, HIGH); //pull ss high to signify end of data transfer
  vspi.endTransaction();
  Serial.print("esp32 master: slave returned: ");
  Serial.println(res);
}
