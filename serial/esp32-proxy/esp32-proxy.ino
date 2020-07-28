

auto& pc = Serial; // the USB
auto& xdev = Serial2;  // external device
#define BAUD 115200
void setup() {
  pc.begin(BAUD);
  xdev.begin(BAUD);
}

void loop() {
  if(pc.available()) { xdev.print((char) pc.read()); }
  if(xdev.available()) { pc.print((char) xdev.read()); }
}
