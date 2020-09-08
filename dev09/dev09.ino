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

hw_timer_t* timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

typedef uint16_t u16;
#define NSINE 64
uint16_t sine[NSINE];

void IRAM_ATTR onTime() {
  static volatile int idx = 0;
  portENTER_CRITICAL_ISR(&timerMux);
  portEXIT_CRITICAL_ISR(&timerMux);
  dac_write(sine[idx++]);
  if(idx>=NSINE) idx=0;
  //Serial.println(idx++);
}


void setup() {
  Serial.begin(115200);
  pinMode(DAC_CS, OUTPUT);
  digitalWrite(DAC_CS, HIGH);
  SPI.begin();

  cmdSetv = cli.addSingleArgCmd("setv", do_setv);
  cmdSetv.setDescription("Set voltage (0-4096) 0-3V3");

 for(int i = 0; i<NSINE; i++) {
  float i1 = (float)i;
  float v = (sin(i1 * 2.0 * 3.1412/(float)NSINE)+1.0)*4096.0/2.0;
  if(v>=4095.0) v = 4095.0;
  if(v<0) v=0;
  //Serial.println(v>0);
  //v = clamp(v, 0.0, 4095.0);
  sine[i] = (uint16_t)v;
  //Serial.println(sine[i]);
 }

 timer = timerBegin(0, 80, true); // prescale so that 1e6 ticks / second
 timerAttachInterrupt(timer, &onTime, true);
 timerAlarmWrite(timer, 1e6 /(440*NSINE), true);
 timerAlarmEnable(timer);
}

void loop() {
  portENTER_CRITICAL(&timerMux);
  portEXIT_CRITICAL(&timerMux);
  
  if(Serial.available()) {
    String input = Serial.readStringUntil('\n');
    cli.parse(input);
  }
}
