
// 2020-06-10 Released into Public Domain
// Use timer2 to output a tone of frequency 440Hz
// Works on Arduino Nano and Uno

// Connect an active speaker (i.e. not a piezo buzzer) between D4 and GND
#define SPKR 4

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

int toggle  = 0;
ISR(TIMER2_COMPA_vect) { //timer0 interrupt 2kHz toggles pin 8
  digitalWrite(SPKR, toggle);
  toggle = 1 - toggle;
}

void setup() {
  Serial.begin(9600);
  pinMode(SPKR, OUTPUT);
  init_timer2(440 * 2);
}

void loop() {
}
