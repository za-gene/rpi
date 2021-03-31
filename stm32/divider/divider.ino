#define IN PB12

static volatile uint32_t cnt = 0;

uint8_t div_pins[] = {
  PB13, PB14, PB15, PA8, PA9, PA10,
  PB5, PB6, PB7, PB8, PB9
};

void flip(int pin, uint32_t when) {
  digitalWrite(pin, (cnt % when) >= (when / 2) );
}

void pin_rising_isr() {
  uint32_t factor = 1;
  for (int i = 0; i < sizeof(div_pins); i++) {
    factor <<= 1;
    flip(div_pins[i], factor);
  }

  cnt++;
}

void output_all() {
  uint8_t all_pins[] = {
    PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7, PA8, PA9, PA10, PA11, PA12, PA15,
    PB0, PB1, PB3, PB4, PB5, PB6, PB7, PB8, PB9, PB10, PB11, PB12, PB13, PB14, PB15,
  };

  for (int i = 0; i < sizeof(all_pins); i++) {
    pinMode(all_pins[i], OUTPUT);
    digitalWrite(all_pins[i], HIGH);
  }
}

void setup() {
  if (0) {
    output_all();
    return;
  }

  pinMode(IN, INPUT);
  attachInterrupt(IN, pin_rising_isr, RISING);
  for (int i = 0; i < sizeof(div_pins); i++) {
    pinMode(div_pins[i], OUTPUT);
  }

}

void loop() {}
