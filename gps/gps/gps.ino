

#include <SoftwareSerial.h>
#include <DogLcd.h>

DogLcd lcd(11, 13, 12, 10); // MOSI, CLK, RS, CS
static char buff[100];

typedef double num_t;

int n(int i) {
  return buff[i] - '0';
}

num_t n(int i, num_t ex) {
  return (buff[i] - '0') * pow(10.0, ex);
}

void display(char* buff) {
  char str[20];
  char str1[20];

  
  // latitude
  num_t deg = n(7, 1) + n(8, 0);
  num_t m = n(9, 1) + n(10) + n(12, -1) + n(13, -2) + n(14, -3) + n(15, -4) + n(16, -5);
  deg = deg + m / 60.0;
  dtostrf(deg, 7, 5, str); // Arduino sprintf doesn't support doubles
  str[8] = 0;
  sprintf(str1, "LAT %s%c  ", str, (char)buff[18]);
  Serial.println(str1);
  lcd.setCursor(0, 0);
  lcd.print(str1);

  // longitude
  deg = n(20, 2) + n(21, 1) + n(22, 0);
  m =  n(23, 1) + n(24, 0) + n(26, -1) + n(27, -2) + n(28,  -3) + n(29, -4) + n(30, -5);
  deg = deg + m / 60;
  dtostrf(deg, 7, 5, str);
  sprintf(str1, "LON %s%c  ", str, (char)buff[32]);
  lcd.setCursor(0, 1);
  lcd.print(str1);

}

void gps_char(char c) {
  static int pos = 0;
  if (c == '\n') c = 0;
  if (pos < sizeof(buff)) buff[pos++] = c;
  if (c != 0) return;
  //Serial.print(".");
  if (strncmp("$GPGLL", buff, 6) == 0) {
    display(buff);
    //Serial.println(buff);
  }
  pos = 0;
}


// The serial connection to the GPS module
SoftwareSerial ss(4, 3); // connect TX to Nano D4, RX to Nano D3

void setup() {
  Serial.begin(9600);
  ss.begin(9600);
  lcd.begin(DOG_LCD_M163);
}

void loop() {
  while (ss.available() > 0) {
    // get the byte data from the GPS
    byte c = ss.read();
    gps_char(c);
  }
}
