#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

typedef uint8_t u8;

static volatile bool read_flag = false;
static volatile u8 vol =0;

ISR(TIMER2_OVF_vect)
{
  static volatile int divider = 0;
  OCR2B = vol;
  if(++divider == 8) {
    read_flag = true;
    divider = 0;    
  } 
}
void setup()
{
  // low-level set-up of timer
  pinMode(3, OUTPUT);
  TCCR2A = 0; // clear it out, because the prescaler is 64, for example
  TCCR2A |= (1<<WGM21) | (1<<WGM20); // Fast PWM. Table 17-8. (WGM22 unset)
  TCCR2A |= (1<<COM2B1); // Clears OC2B on compare match, set OC2B at bottom, non-inverting mode
   
  TCCR2B = 0;
  TCCR2B |= (1<<CS20);       // Prescale 1, giving ~ 62500Hz

  // ignore duty cycle for this sketch. The timer sets it

  TIMSK2 |= (1<<TOIE2); //enable overflow interrupt for timer2


  setup_stuff();

}

File dataFile;
bool opened = false;

void loop()
{
  if(read_flag && opened) {
    read_flag = false;
   
    if(dataFile.available()) {
      //Serial.print(".");
      vol = dataFile.read();
    } else {
      vol = 0;
      dataFile.seek(0);
    }
    
  }
}


const int chipSelect = 8;

void setup_stuff() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
 
  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  dataFile = SD.open("song.raw");
  if(dataFile) {
    opened = true;
  } else {
    Serial.println("Couldn't open file");
  }


}
