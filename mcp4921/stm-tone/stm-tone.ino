/* Designed for a basic test of MCP4921.
 * Use an LED on the DAC output pin (pin 8), just to see things working.
 * Hookup is as per sketch_jun13b
 * dac_write() takes about 8.4us, for a max freq of 119kHz
 * 
 */

#include <SPI.h>


auto& ser = Serial1;

#define DAC_CS PB12
SPIClass dacspi(2); // CS=PB12, SCK=PB13, MOSI=PB15, no MISO
//SPISettings spi_settings(20e6, MSBFIRST, SPI_MODE0);

void dac_write(uint16_t value) {
  if(value>4095) value = 4095;
  value |= 0b0011000000000000;
  //dacspi.beginTransaction(spi_settings );
  digitalWrite(DAC_CS, LOW);
  dacspi.transfer16(value);
  digitalWrite(DAC_CS, HIGH);
  //dacspi.endTransaction();
}




void setup() {

  ser.begin(115200);
  ser.println(MOSI);
  ser.println(MISO);
  ser.println(SCK);
  ser.println(SS);

  pinMode(DAC_CS, OUTPUT);
  digitalWrite(DAC_CS, HIGH);
  dacspi.begin();
  dacspi.setBitOrder(MSBFIRST);
  dacspi.setDataMode(SPI_MODE0);

  auto ms = micros();
  for (int i = 0; i < 1000; i++) {
    dac_write(4095);
  }
  ms = micros() - ms;
  ser.print("1000 dac_writes took (us): ");
  ser.println(ms);


}
void loop() {
  static int vol = 0;
  //vol = 4095;
  dac_write(vol);
  vol = 4095 - vol;
  delayMicroseconds(500); // fairly aritrary
  //dac_write(random(4096));
  //if (++i > 4095) i = 0;
  //delay(1);
}
