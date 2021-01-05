#include<SPI.h>

void setup() {
  pinMode(MISO, OUTPUT);  //Sets MISO as OUTPUT (Have to Send data to Master IN
  pinMode(MOSI, INPUT); 
  SPCR |= _BV(SPE);       //Turn on SPI in Slave Mode
  SPI.attachInterrupt();  //Interuupt ON is set for SPI commnucation
  Serial.begin(115200);
}


volatile uint8_t cnt = 0;

ISR(SPI_STC_vect)
{
  volatile byte c = SPDR;
  SPDR = cnt;
  Serial.println("Slave recd " + String(c) + ", sent " + String(cnt));
  cnt++;
}

void loop(){}
