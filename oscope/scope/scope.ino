/*
 * For a normal setup with the default prescaler, 1024 samples takes ~65,000us.
 * So each sample takes ~63us (=65000us/1024), for a sampling frequency of ~15.8kHz (= 1/63e-6)
 */
#define AIN A0
//#define PIN_440 2

template<int N>
struct Buffer {
  int capacity = N;
  uint16_t data[N];
  int size = 0;
  int avail() {
    return capacity - size;
  }
  void write(uint16_t v) {
    if (size < capacity) {
      data[size++] = v;
    }
  }
};

Buffer<512> buff;

void setup() {
  Serial.begin(115200);
  pinMode(AIN, INPUT);
  bool diagnostics = false;

  //tone(PIN_440, 440);

  if (diagnostics) {
    Serial.println("+OK OSCOPE Service Ready");
    Serial.println("+OK Capturing. Wait.");
  }
  //while(analogRead(AIN) < 100);
  
  auto ms = micros();
  while (buff.avail()) buff.write(analogRead(AIN));
  ms = micros() - ms;
  String str = "Took " + String(ms) + " us";
  if (diagnostics) Serial.println(str);
  for (int i = 0; i < buff.size; ++ i)
    Serial.println(buff.data[i]);
  //Serial.println(".");
  //Serial.flush();

}

void loop() {
  // put your main code here, to run repeatedly:
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\r')
      Serial.print("\r\n");
    else
      Serial.print(c);
  }

}
