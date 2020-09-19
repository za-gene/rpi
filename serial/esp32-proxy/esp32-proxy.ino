

auto& pc = Serial; // the USB
auto& xdev = Serial2;  // external device

#if 1
#define BAUD 115200
#else
#define BAUD 9600
#endif

void setup() {
  pc.begin(BAUD);
  xdev.begin(BAUD);
}

void loop() {
  if(pc.available()) { xdev.print((char) pc.read()); }
  if(xdev.available()) { pc.print((char) xdev.read()); }
}
