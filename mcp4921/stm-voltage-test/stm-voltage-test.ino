/* Designed for a basic test of MCP4921.
 * Use an LED on the DAC output pin (pin 8), just to see things working.
 * Hookup is as per sketch_jun13b
 * dac_write() takes about 8.4us, for a max freq of 119kHz
 * 
 */

#include <SPI.h>

#include <debounce.h>

#define BTN PA5
FallingButton btn(BTN);

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
  /*
  ser.println(MOSI);
  ser.println(MISO);
  ser.println(SCK);
  ser.println(SS);
*/

  pinMode(DAC_CS, OUTPUT);
  digitalWrite(DAC_CS, HIGH);
  dacspi.begin();
  dacspi.setBitOrder(MSBFIRST);
  dacspi.setDataMode(SPI_MODE0);

 

}


int values[] = {0, 2048, 4095};

void loop() {
  static int vidx = 0;
  if(btn.falling()) {
    uint16_t v = values[vidx++];
    ser.println(v);
    if(vidx>2) vidx = 0;
    dac_write(v);
  }
  //ser.println(digitalRead(BTN));
  //delay(500);
}
