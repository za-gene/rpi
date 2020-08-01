/*
  SD card file dump

 This example shows how to read a file from the SD card using the
 SD library and send it over the serial port.

 The circuit:
 * SD card attached to SPI bus as follows:
 **       SD NANO   STM32
 ** MOSI  D7   11   PA7
 ** MISO  D6   12   PA6
 ** CLK   D5   13   PA5
 ** CS -  D8    4   PA4
 
 * Fore Serial1, STM32 TX1 is PA9, RX1 is PA10. Remember to cross-over.
 
 created  22 December 2010 by Limor Fried
 modified 9 Apr 2012 by Tom Igoe
 modified 2020-08-01 by Mark Carter

 This example code is in the public domain.

 */

#include <SPI.h>
#include <SD.h>

auto& ser = Serial1;
const int chipSelect = PA4; // For STM32

void setup() {
  // Open serial communications and wait for port to open:
  ser.begin(115200);
  //while (!ser) {
    ; // wait for serial port to connect. Needed for native USB port only
  //}


  ser.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    ser.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  ser.println("card initialized.");

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("stm32.txt");

  // if the file is available, write to it:
  if (dataFile) {
    while (dataFile.available()) {
      ser.write(dataFile.read());
    }
    dataFile.close();
  }
  // if the file isn't open, pop up an error:
  else {
    ser.println("error opening file");
  }
}

void loop() {
}
