#include <debounce.h>

#define led 6
FallingButton sw(7);

void setup() {
  pinMode(led, OUTPUT);
}

void loop() {
  if (sw.falling()) digitalWrite(led, 1 - digitalRead(led));
}
