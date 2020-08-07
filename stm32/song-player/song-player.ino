/*
 * Plays a song in raw format: 8bit samples, 8kHz rate
 * 
 * SD CARD -> STM32: MOSI D7 -> PA7, MISO D6 -> PA6, CLK D5 -> PA5. CS D8 -> PA4. also: 3V3 (!) and GND
 * 
 * MCP4921 -> STM32: VDD 1 -> 3V3, CS 2 -> PB12, SCK 3 ->PB13, MOSI 4 -> PB15, 
 * ... LDAC 5 -> GND, VREFA 6 -> 3V3, AVSS 7 -> GND, VOUTA 8 -> SPK
 * 
 * Speaker (SPK) should go through OPAM)
 * 
 */

#include <SPI.h>
#include <SD.h>
 
#define LED PB8
auto& ser = Serial1;
const int sd_cs = PA4; // SD card chip select

HardwareTimer timer(2);
File dataFile;

#define DAC_CS PB12
SPIClass dacspi(2); // CS=PB12, SCK=PB13, MOSI=PB15, no MISO

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
  pinMode(LED, OUTPUT);

  pinMode(DAC_CS, OUTPUT);
  digitalWrite(DAC_CS, HIGH);
  dacspi.begin();
  dacspi.setBitOrder(MSBFIRST);
  dacspi.setDataMode(SPI_MODE0);

  ser.print("Initializing SD card...");
  if (!SD.begin(sd_cs)) {
    ser.println("FAIL");
    while (1);
  }
  ser.println("OK");

  ser.print("Opening song...");
  dataFile = SD.open("song.raw");
  if(!dataFile) {
    ser.println("FAIL");
    while(1);
  }
  ser.println("OK");
  

  timer.pause();
  timer.setPeriod(125); // in microseconds. 125us equates to 8000Hz
  timer.setChannel1Mode(TIMER_OUTPUT_COMPARE);
  timer.setCompare(TIMER_CH1, 1);  // Interrupt 1 count after each update
  timer.attachCompare1Interrupt(tmr_handler);
  timer.refresh(); // Refresh the timer's count, prescale, and overflow
  timer.resume(); // start the timer counting

}

void loop() {
}


int led_idx = 0;
void tmr_led() {
  if (led_idx++ == 0) digitalWrite(LED, HIGH);
  if (led_idx == 100) digitalWrite(LED, LOW); // turn it off after 100*125us
  if (led_idx == 8000) led_idx = 0; // corresponding to our clock frequency
}

uint16_t vol = 0;
void tmr_handler() {
  dac_write(vol);
  if(dataFile.available()) {
    vol = dataFile.read() << 4; // 8-bit to 12-bit
  } else {
    dataFile.seek(0);
  }
  tmr_led();
}
