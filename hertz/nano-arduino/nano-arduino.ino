#include <MsTimer2.h>

typedef uint32_t u32;
//typedef uint16_t u16;

volatile bool allow_count = false;
volatile u16 count = 0;
const byte pin = 3;

void counter_isr() {
  if(allow_count) count++;  
}
void report_isr() {
  //if(!allow_count) goto skip; // skip the first  encounter
  allow_count = false;
  volatile u16 cnt = count;
  count = 0;
  allow_count = true;
  Serial.println(cnt);

}

void setup() {
  Serial.begin(115200);
  pinMode(pin, INPUT);
  MsTimer2::set(1000, report_isr);
  attachInterrupt(digitalPinToInterrupt(pin), counter_isr, FALLING);
  MsTimer2::start();

}

void loop() {
}
