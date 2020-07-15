void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(A0, INPUT);
  auto ms = micros();
  for (int i = 0; i < 1000; ++i)
    analogRead(A0);
  ms = micros() - ms;
  Serial.println(ms);

  ms = micros();
  for (int i = 0; i < 1000; ++i)
    digitalRead(A0);
  ms = micros() - ms;
  Serial.println(ms);

  pinMode(8, OUTPUT);
  ms = micros();
  for (int i = 0; i < 1000; ++i)
    digitalWrite(8, 0);
  ms = micros() - ms;
  Serial.println(ms);
}

void loop() {
  // put your main code here, to run repeatedly:

}
