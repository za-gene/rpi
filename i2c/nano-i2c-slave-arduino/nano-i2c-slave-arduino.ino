#include <Wire.h>

#define I2C_SLAVE_ADDRESS 0x4

int i=0;

void setup() {
  Wire.begin(I2C_SLAVE_ADDRESS);
  Wire.onRequest(requestEvent);
  //pinMode(PB4, 1);
  //digitalWrite(PB4, HIGH);
}

void loop() {
}

void requestEvent()
{
  Wire.write(i);
  i++;  
}
