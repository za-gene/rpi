#include<SPI.h>

void setup() {
  pinMode(MISO, OUTPUT);                  //Sets MISO as OUTPUT (Have to Send data to Master IN
  SPCR |= _BV(SPE);                       //Turn on SPI in Slave Mode
  SPI.attachInterrupt();                  //Interuupt ON is set for SPI commnucation
}


volatile uint8_t cnt = 0;

ISR(SPI_STC_vect)
{
  byte c = SPDR; //received
  SPDR = cnt++;
}

void loop(){}
