#include "song.h"
#define DAC_CS 5
#include <SPI.h>

//#define USE_INTERNAL_DAC

//volatile SPISettings spi_settings(4000000, MSBFIRST, SPI_MODE0);
//SPISettings spi_settings(16e6, MSBFIRST, SPI_MODE0);
SPISettings spi_settings(20e6, MSBFIRST, SPI_MODE0);

void IRAM_ATTR dac_write(uint16_t value) {
  //  void  dac_write(uint16_t value) {
  //byte b0 = highByte(value);
  //byte b0 = 0b00001111 & (value >> 8);
  //b0 = 0b00110000 | b0; // add command
  //byte b1 = value & 0xFF;
  value &= 0b0000111111111111;
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


extern unsigned char song_raw[];


//portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;




volatile int idx = 0;
void IRAM_ATTR onTimer() {
  //portENTER_CRITICAL_ISR(&timerMux);
#ifdef USE_INTERNAL_DAC
  dacWrite(25, song_raw[idx++]);
#else

  dac_write(((uint16_t)song_raw[idx++]) << 3);
  //dac_write(random(4096));

#endif
  if (idx >= song_raw_len) idx = 0;
  //idx = idx % song_raw_len;
  //portEXIT_CRITICAL_ISR(&timerMux);
}

hw_timer_t * timer = NULL;
void setup() {

  Serial.begin(9600);
  Serial.println(MOSI);
  Serial.println(MISO);
  Serial.println(SCK);
  Serial.println(SS);
  //Serial.println(RST);

  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1e6 / 8000, true);
  timerAlarmEnable(timer);

  //pinMode(SPKR, OUTPUT);
  pinMode(DAC_CS, OUTPUT);
  digitalWrite(DAC_CS, HIGH);
  SPI.begin();


}
void loop() {
  //delay(1000); // seems necessary to avoid guru meditation when using MCP4921
}
