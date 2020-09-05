#define DAC_CS 5
#include <SPI.h>

#include <SimpleCLI.h>

SimpleCLI cli;
Command cmdSetv;

SPISettings spi_settings(20e6, MSBFIRST, SPI_MODE0);

void dac_write(uint16_t value) {
  if(value>4095) value = 4095;
  value |= 0b0011000000000000;
  SPI.beginTransaction(spi_settings );
  digitalWrite(DAC_CS, LOW);
  SPI.transfer16(value);
  digitalWrite(DAC_CS, HIGH);
  SPI.endTransaction();
}


void do_setv(cmd* c) {
  Command cmd(c);
  Argument arg = cmd.getArgument(0);
  int v = arg.getValue().toInt();
  Serial.print("set voltage: ");
  Serial.println(v);
  dac_write(v);  
}

void setup() {
  Serial.begin(115200);
  pinMode(DAC_CS, OUTPUT);
  digitalWrite(DAC_CS, HIGH);
  SPI.begin();

  cmdSetv = cli.addSingleArgCmd("setv", do_setv);
  cmdSetv.setDescription("Set voltage (0-4096) 0-3V3");
}

void loop() {
  if(Serial.available()) {
    String input = Serial.readStringUntil('\n');
    cli.parse(input);
  }
}
