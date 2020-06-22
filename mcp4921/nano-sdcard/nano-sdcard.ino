/*
   Media player

   Hardware:
   SD Card
   MCP4921 (12-bit audio DAC)
   Arduino Nano/Uno

   This example code is in the public domain.
*/

#include <SPI.h>
#include <SD.h>

// Chip (aka Slave) Select for SD card and DAC. Change if required
const int SD_CS = 4;
const int DAC_CS = 2;

SPISettings dac_spi_settings(16e6, MSBFIRST, SPI_MODE0);

void dac_write(uint16_t value) {
  value &= 0b0000111111111111;
  value |= 0b0011000000000000;
  SPI.beginTransaction(dac_spi_settings);
  digitalWrite(DAC_CS, LOW);
  SPI.transfer16(value);
  digitalWrite(DAC_CS, HIGH);
  SPI.endTransaction();
}

void  init_timer2(unsigned int f)
{
  static const unsigned int scales[] = {1, 8, 32, 64, 128, 256, 1024};

  cli();//stop interrupts
  TCCR2A = 0;
  TCNT2  = 0;
  TCCR2A |= (1 << WGM21); // turn on CTC mode
  TIMSK2 |= (1 << OCIE2A); // enable timer compare interrupt

  // calculate prescaler and Match Register Comparison
  unsigned int p;
  for (p = 0; p < 8; p++) if (scales[p] >= 62500 / f) break;
  OCR2A = (62500 / scales[p]) * 256 / f; // Set Compare Match Register
  TCCR2B = ++p; // the prescaler

  sei();//allow interrupts
}

File dataFile;

void setup() {
  Serial.begin(9600);
  Serial.print("Initializing SD card...");
  if (!SD.begin(SD_CS)) {
    Serial.println("FAIL ... Aborting");
    return;
  }
  Serial.println("OK");

  Serial.print("Opening file...");
  dataFile = SD.open("song.raw");
  if (!dataFile) {
    Serial.println("FAIL ... Aborting");
    return;
  }
  Serial.println("OK");


  pinMode(DAC_CS, OUTPUT);
  digitalWrite(DAC_CS, HIGH);
  init_timer2(8000); // set sampling frequency 8kHz
}


ISR(TIMER2_COMPA_vect) {
  if (!dataFile.available()) {
    dataFile.seek(0);
  }
  dac_write(dataFile.read() << 4); // convert a byte value to a 12-bit value
}

void loop() {
}
