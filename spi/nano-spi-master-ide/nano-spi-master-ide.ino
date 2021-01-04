#include<SPI.h>

#define CS 10

void setup() {
  pinMode(CS, OUTPUT);
  digitalWrite(CS, HIGH);
  Serial.begin(115200);
  SPI.begin();
}

void loop() {
  digitalWrite(CS, LOW);
  int val = SPI.transfer(666);
  digitalWrite(CS, HIGH);
  Serial.println(val);
  delay(1000);

}
