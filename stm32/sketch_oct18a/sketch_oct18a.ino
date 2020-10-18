HardwareTimer tim3(3);

void setup() {
  pinMode(PA6, PWM);
  tim3.setPeriod(10000); // in uint32_t microseconds. 100Hz in this case
  pwmWrite(PA6, 20000); // uint32_t. duty cycle of 20000/65535 = 30.5%
}

void loop() {
}
