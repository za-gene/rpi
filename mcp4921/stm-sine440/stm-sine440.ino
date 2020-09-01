#include <SPI.h>

typedef uint16_t u16;
auto& ser = Serial1;

HardwareTimer timer(2);

#define DAC_CS PB12
SPIClass dacspi(2); // CS=PB12, SCK=PB13, MOSI=PB15, no MISO
//SPISettings spi_settings(20e6, MSBFIRST, SPI_MODE0);

void dac_write(uint16_t value) {
  if(value>4095) value = 4095;
  value |= 0b0011000000000000;
  //dacspi.beginTransaction(spi_settings );
  digitalWrite(DAC_CS, LOW);
  dacspi.transfer16(value);
  digitalWrite(DAC_CS, HIGH);
  //dacspi.endTransaction();
}


#define SAMPLES 100
u16 sines[SAMPLES];
void setup() {
  ser.begin(115200);

  ser.println("Sine wave is:");
  for (int i = 0; i < SAMPLES; i++) {
    double v = (sin((double) i * 2.0 * 3.1412 / SAMPLES) + 1.0) * 4096.0 / 2.0;
    //v = clamp(0, 4095);
    sines[i] = v;
    ser.println(sines[i]);
  }

  pinMode(DAC_CS, OUTPUT);
  digitalWrite(DAC_CS, HIGH);
  dacspi.begin();
  dacspi.setBitOrder(MSBFIRST);
  dacspi.setDataMode(SPI_MODE0);


  timer.pause();
  double freq = 440 * SAMPLES;
  timer.setPeriod(1E6/freq); // in microseconds. 
  timer.setChannel1Mode(TIMER_OUTPUT_COMPARE);
  timer.setCompare(TIMER_CH1, 1);  // Interrupt 1 count after each update
  timer.attachCompare1Interrupt(tmr_handler);
  timer.refresh(); // Refresh the timer's count, prescale, and overflow
  timer.resume(); // start the timer counting

  
}

void loop() {
  //static int i = 0;
  //ser.println(i++);
  //delay(2000);
}

volatile int idx=0;
void tmr_handler() {
  dac_write(sines[idx]);
  idx++;
  if(idx>=SAMPLES) idx = 0;
}
