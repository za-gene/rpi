
#include <SPI.h>

#define SS PA4
static const int spiClk = 250000; // 250kHz

auto& ser = Serial1;
void setup() {
  pinMode(SS, OUTPUT);
  digitalWrite(SS, HIGH);
  
  ser.begin(115200);
  ser.println("Hello from stm32 spi master");
  
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV8);//divide the clock by 8  
}

void loop() {
  byte data = 0b01010101; // junk data to illustrate usage

  digitalWrite(SS, LOW); //pull SS slow to prep other end for transfer
  int res = SPI.transfer(data);  
  digitalWrite(SS, HIGH); //pull ss high to signify end of data transfer
  ser.print("stm32 master: slave returned: ");
  ser.println(res);
  delay(1000);
}
