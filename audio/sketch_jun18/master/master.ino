// I2C master ESP32


#include <Wire.h>

#define I2C_ADDR 4
//#define SPEED 1000000 // fast mode
#define SPEED 3200000 //  mode

hw_timer_t * timer = NULL;

#define BUFSIZE 8
volatile byte  dbuf[2][BUFSIZE];
volatile uint8_t playing = 0, filling = 1, bidx = 0;

void IRAM_ATTR onTimer() {
  dacWrite(25, dbuf[playing][bidx++]);
  if(bidx>=BUFSIZE) {
    bidx = 0;
    playing = 1 - playing;
  }
}

void setup() {
  Wire.begin();
  Wire.setClock(SPEED);
  int serial_speed;
  serial_speed = 115200;
  serial_speed = 9600;
  Serial.begin(serial_speed);
    timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 125, true);
  timerAlarmEnable(timer);
 
}

void loop() {
  uint8_t playchan = playing;
  if (filling != playchan) {
    Wire.requestFrom(I2C_ADDR, BUFSIZE);
    for(uint8_t i=0; i< BUFSIZE; ++i) {
      //while(!Wire.available()) ;
      dbuf[filling][i] = Wire.read();
    }
    filling = playchan;
  }
}
