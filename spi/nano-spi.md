# nano spi

Confirmed working 2021-01-04

Hookup, identical for both master and slave: 
```
D10 CS 
D11 MOSI
D13 CLK
```

MISO unused in this example.



[nano-spi-master-ide](nano-spi-master-ide) implements a reading SPI master:

```
#include<SPI.h>

#define CS 10

void setup() {
  pinMode(CS, OUTPUT);
  digitalWrite(CS, HIGH);
  Serial.begin(115200);
  SPI.begin();
}

void loop() {
  digitalWrite(CS, LOW);
  int val = SPI.transfer(666);
  digitalWrite(CS, HIGH);
  Serial.println(val);
  delay(1000);
}
```


 
[nano-spi-slave-ide](nano-spi-slave-ide) implements the counterpart writing SPI slave:

```
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
```


