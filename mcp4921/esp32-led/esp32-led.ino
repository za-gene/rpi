/* Designed for a basic test of MCP4921.
 * Use an LED on the DAC output pin (pin 8), just to see things working.
 * Hookup is as per sketch_jun13b
 * dac_write() takes about 8.4us, for a max freq of 119kHz
 * 
 */
#define DAC_CS 5
#include <SPI.h>


//volatile SPISettings spi_settings(4000000, MSBFIRST, SPI_MODE0);
//SPISettings spi_settings(16e6, MSBFIRST, SPI_MODE0);
SPISettings spi_settings(20e6, MSBFIRST, SPI_MODE0);

void dac_write(uint16_t value) {
  if(value>4095) value = 4095;
  //  void  dac_write(uint16_t value) {
  //byte b0 = highByte(value);
  //byte b0 = 0b00001111 & (value >> 8);
  //b0 = 0b00110000 | b0; // add command
  //byte b1 = value & 0xFF;
  //value &= 0b0000111111111111;
  value |= 0b0011000000000000;
  //noInterrupts();
  SPI.beginTransaction(spi_settings );
  digitalWrite(DAC_CS, LOW);
  //SPI.transfer(b0);
  //SPI.transfer(b1);
  SPI.transfer16(value);
  digitalWrite(DAC_CS, HIGH);
  SPI.endTransaction();
  //interrupts();
}




void setup() {

  Serial.begin(115200);
  Serial.println(MOSI);
  Serial.println(MISO);
  Serial.println(SCK);
  Serial.println(SS);

  pinMode(DAC_CS, OUTPUT);
  digitalWrite(DAC_CS, HIGH);
  SPI.begin();

  auto ms = micros();
  for (int i = 0; i < 1000; i++) {
    dac_write(4095);
  }
  ms = micros() - ms;
  Serial.print("1000 dac_writes took (us): ");
  Serial.println(ms);


}
void loop() {
  static int i = 0;
  dac_write(i);
  if (++i > 4095) i = 0;
  delay(1);
}
