#include <SPI.h>

#define NTAB 32
volatile uint16_t sine_table[NTAB];
#define DAC_CS 2
SPISettings dac_spi_settings(100000, MSBFIRST, SPI_MODE0);

auto& ser= Serial;


void  init_timer2(unsigned int f)
{
  static const unsigned int scales[] = {1, 8, 32, 64, 128, 256, 1024};

  cli();//stop interrupts
  TCCR2A = 0;
  TCNT2  = 0;
  TCCR2A |= (1 << WGM21); // turn on CTC mode
  TIMSK2 |= (1 << OCIE2A); // enable timer compare interrupt

  // calculate prescaler and Match Register Comparison
  unsigned int p;
  for (p = 0; p < 8; p++) if (scales[p] >= 62500 / f) break;
  OCR2A = (62500 / scales[p]) * 256 / f; // Set Compare Match Register
  TCCR2B = ++p; // the prescaler

  sei();//allow interrupts
}

void dac_write(uint16_t value) {
  if(value>4095) value = 4095;
  //value &= 0b0000111111111111;
  value |= 0b0011000000000000;
  SPI.beginTransaction(dac_spi_settings);
  digitalWrite(DAC_CS, LOW);
  SPI.transfer16(value);
  digitalWrite(DAC_CS, HIGH);
  SPI.endTransaction();
}

volatile int idx=0;
/*
ISR(TIMER2_COMPA_vect) {
  dac_write(sine_table[idx++]);
  if(idx>=NTAB) idx = 0;
  //dac_write(4096);
}
*/

void setup() {
  // put your setup code here, to run once:
  ser.begin(115200);
  for(int i=0; i<NTAB; ++i) {
    float f = 2.0 * PI * i / NTAB;
    float s = (1.0+sin(f))*4096/2;
    int si = (int)s;
    //if(si == 4096) s=4095;
    //si = si >> 1;
    sine_table[i] = si;
    char str[30];
    //sprintf(str, "%d %d", i, s);
    //ser.println(str);
    ser.print(i); ser.print(" "); ser.println(si);
  }

  pinMode(DAC_CS, OUTPUT);
  digitalWrite(DAC_CS, HIGH);
  //init_timer2(44 * TAB);
  dac_write(4095);
}

void loop() {
  // put your main code here, to run repeatedly:

}
