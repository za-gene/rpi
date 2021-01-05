/*
 * A SPI master receiver for the Nano using sw (software) bitbanging
 * 
 * 2021-01-05 created. Confirmed working
 */
#define CS    10
#define CLK   13
#define MISO  12

void setup() {
  pinMode(CS, OUTPUT);
  digitalWrite(CS, HIGH);
  pinMode(CLK, OUTPUT);
  digitalWrite(CLK, LOW);
  pinMode(MISO, INPUT);
  Serial.begin(115200);
}

void pause(int n = 10) { delayMicroseconds(n); }

void loop() {
  uint8_t val;
  digitalWrite(CS, LOW);
  pause();
  for(int i=0; i<8; i++) {
    digitalWrite(CLK, HIGH);
    pause();
    val = (val<<1) + digitalRead(MISO);
    //pause(5);
    digitalWrite(CLK, LOW);
    pause();            
  }
  digitalWrite(CS, HIGH);
  pause();
  Serial.println(val);
  delay(1000);
}
