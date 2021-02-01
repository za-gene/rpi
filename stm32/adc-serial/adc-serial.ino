#define AIN     PB1
#define PWM_PIN PB6
typedef uint16_t u16;


HardwareTimer pwmtimer4(4);

auto& ser = Serial1;
void setup()
{

  ser.begin(115200);
  pinMode(AIN, INPUT_ANALOG);

pinMode(PWM_PIN, PWM);
  pwmtimer4.pause();
  pwmtimer4.setOverflow(20000);
  pwmtimer4.refresh();
  pwmtimer4.resume();

}

//uint8_t countr = 0;
void loop()
{
  static u16 prev = 0;
  u16 reading = 0;
  for (int i = 0 ; i < 10; i++) reading += analogRead(AIN);
  reading /= 10;
  if (abs(prev - reading) > 5) {
    prev = reading;

  }
  //reading >>= 2;
  ser.println(reading);
  delay(1000);

}
