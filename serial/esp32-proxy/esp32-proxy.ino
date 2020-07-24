

auto& pc = Serial; // the USB
auto& xdev = Serial2;  // external device
void setup() {
  pc.begin(9600);
  xdev.begin(9600);
}

void loop() {
  if(pc.available()) { xdev.print((char) pc.read()); }
  if(xdev.available()) { pc.print((char) xdev.read()); }
}
