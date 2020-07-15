#define AIN A0
#define PIN_440 2

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

Buffer<128> buff;

void setup() {
  Serial.begin(115200);
  pinMode(AIN, INPUT);
  tone(PIN_440, 440);
  auto ms = micros();

  Serial.println("+OK OSCOPE Service Ready");

  Serial.println("+OK Capturing. Wait.");
  while (buff.avail()) buff.write(analogRead(AIN));
  ms = micros() - ms;
  String str = "Took " + String(ms) + " us";
  Serial.println(str);
  for (int i = 0; i < buff.size; ++ i)
    Serial.println(buff.data[i]);
  Serial.println(".");
  Serial.flush();

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
