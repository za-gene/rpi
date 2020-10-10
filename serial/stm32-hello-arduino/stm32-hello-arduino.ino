// this connects you to /dev/ttyACM0 
auto& ser = Serial;

void setup() {
  ser.begin(115200);

}

void loop() {
  static int i = 0;
  ser.println(i++);
  delay(1000);
}
