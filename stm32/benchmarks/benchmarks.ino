#define W_PIN PA0
#define R_PIN PA1
#define A_PIN PA2

void timeit(char* desc, int n) {
  auto ms = micros();
  for (int i = 0; i < 10000; i++) {
    switch (n) {
      case 1 : digitalWrite(W_PIN, HIGH); break;
      case 2 : digitalRead(W_PIN); break;
      case 3: analogRead(A_PIN); break;
    }
  }
  ms = micros() - ms;
  Serial1.print("10000 ");
  Serial1.print(desc);
  Serial1.print(" took ");
  Serial1.print(ms);
  Serial1.println(" micros");
}

void setup() {
  Serial1.begin(9600);

  pinMode(W_PIN, OUTPUT);
  timeit("digitalWrite()", 1);

  pinMode(R_PIN, INPUT);
  timeit("digitalRead()", 2);

  pinMode(A_PIN, INPUT_ANALOG);
  timeit("analogRead()", 3);

  Serial1.println("");

}

void loop() {
  // put your main code here, to run repeatedly:
  static int c = 0;
  //Serial1.println(c++);
  delay(1000);

}
