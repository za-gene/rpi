#include <debounce.h>

#define led LED_BUILTIN
Debounce sw(7);

void setup() {
  Serial.begin(115200);
  pinMode(led, OUTPUT);
}

void loop() {
  if (sw.falling()){
    Serial.println("Falling");
    digitalWrite(led, 1 - digitalRead(led));
  }
}
