

auto& ser = Serial2;
void setup() {
  Serial.begin(9600);
  ser.begin(9600);
}

void loop() {
  while(ser.available()) {
    Serial.print((char)ser.read());
  }
}
