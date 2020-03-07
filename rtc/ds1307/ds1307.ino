#include <DS1307RTC.h> 

/* DS1307 -> WROOM connection:
 * GND -> GND
 * VCC -> 3V3 
 * SDA -> D21
 * SCL -> D22
 */

DS1307RTC& DS = RTC; // create an alias to avoid confusion between internal clock and external clock


// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
}

// the loop function runs over and over again forever
void loop() {
  time_t t = DS.get();
  printDateTime(t);
  delay(1000);
}

void printDateTime(time_t t)
{
  char buf[32];
  char m[4];    // temporary storage for month string (DateStrings.cpp uses shared buffer)
  strcpy(m, monthShortStr(month(t)));
  sprintf(buf, "%.2d:%.2d:%.2d %s %.2d %s %d",
          hour(t), minute(t), second(t), dayShortStr(weekday(t)), day(t), m, year(t));
  Serial.println(buf);
}
