// This example code is in the public domain.
// SD card speed test


#include <SPI.h>
#include <SD.h>

#define SD_CS 4


char block[512];
File dataFile;

auto ms = micros();
void timr_on() {
  ms = micros();
}

void timr_off(int bsize) {
  auto ms1 = micros();
  Serial.print(bsize);
  auto len = ms1 - ms;
  Serial.print('\t');
  Serial.print(len);
  Serial.println();
  dataFile.seek(0);
}
void read_size(int bsize) {
  timr_on();
  for (int i = 0; i < 8192 / bsize; ++i) {
    dataFile.read(block, bsize);
  }
  timr_off(bsize);
}


void setup()
{
  Serial.begin(9600);           // start serial for output
  if (!SD.begin(SD_CS)) {
    Serial.println("Card failed, or not present");
    return;
  }
  Serial.println("card initialized.");

  dataFile = SD.open("song.raw");
  if (dataFile)
    Serial.println("Song opened OK");
  else
    Serial.println("Song open fail");

  char c;
  timr_on();
  for (int i = 0; i < 8192; ++i) {
    c = dataFile.read();
  }
  timr_off(1);

  read_size(8);
  read_size(16);
  read_size(256);
  read_size(512);


}

void loop()
{
}
