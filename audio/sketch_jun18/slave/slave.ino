// This example code is in the public domain.
// I2C slave Nano


#include <Wire.h>
#include <SPI.h>
#include <SD.h>


#define I2C_ADDR 4
#define SPEED 1000000
#define SPEED 3200000
#define SD_CS 4

#define BUFSIZE 8

byte buffer[BUFSIZE];
int idx = 0;

File dataFile;
void setup()
{
  Wire.begin(I2C_ADDR);
  Wire.setClock(SPEED);
  Wire.onRequest(requestEvent);
  Serial.begin(9600);
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

}

void loop()
{
}

void requestEvent()
{
  bool ok = dataFile.available();
  if (ok) {
    dataFile.read(buffer, sizeof(buffer));
  } else {
    memset(buffer, 0, sizeof(buffer));
  }

  Wire.write(buffer, sizeof(buffer));

  if (!ok) {
    dataFile.seek(0); // rewind back to start of the file
  }
}
