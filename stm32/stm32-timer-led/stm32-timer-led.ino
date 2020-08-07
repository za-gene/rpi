// blinks every second using interrupts

#define LED PB8

HardwareTimer timer(2);

void setup() {
  pinMode(LED, OUTPUT);
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


int idx=0;
void tmr_handler() {
  if(idx++==0) digitalWrite(LED, HIGH);
  if(idx==100) digitalWrite(LED, LOW); // turn it off after 100*125us
  if(idx==8000) idx =0; // corresponding to our clock frequency    
}
