//#if CONFIG_FREERTOS_UNICORE
//#define ARDUINO_RUNNING_CORE 0
//#else
//#define ARDUINO_RUNNING_CORE 1
//#endif

#include <NTPClient.h>
// change next line to use with another board/shield
//#include <ESP8266WiFi.h>
#include <WiFi.h> // for WiFi shield
//#include <WiFi101.h> // for WiFi 101 shield or MKR1000
#include <WiFiUdp.h>

#include <DS1307RTC.h> // https://github.com/PaulStoffregen/DS1307RTC
#include <Timezone.h> // https://github.com/JChristensen/Timezone


#include "/home/pi/repos/redact/docs/secret/settings.h"
const char *ssid     = WIFI_SSID;
const char *password = WIFI_PASSWORD;

WiFiUDP wifiUDP;
NTPClient ntpClient(wifiUDP, "uk.pool.ntp.org");

DS1307RTC& DS = RTC; // create an alias to avoid confusion between internal clock and external clock

enum BlinkttPin { alive = 0, ds, wifi, ntp};
void  set_blinkt(uint8_t n, bool ok);

TimeChangeRule myBST = {"BST", Last, Sun, Mar, 1, 60};
TimeChangeRule mySTD = {"GMT", Last, Sun, Nov, 2, 0};
Timezone myTZ(myBST, mySTD);
TimeChangeRule *tcr;        //pointer to the time change rule, use to get TZ abbrev

void delay_ms(int n) {
  vTaskDelay(n / portTICK_PERIOD_MS);
}
void delay_sec(int n) {
  for (int i = 0; i < n; i++) delay_ms(1000);
}
void delay_min(int n) {
  for (int i = 0; i < n; i++) delay_sec(60);
}

void TaskWifi( void *pvParameters )
{
  for (;;) {
    bool ok = WiFi.status() == WL_CONNECTED;
    set_blinkt(wifi, ok);
    if (ok) {
      delay_min(2);
    } else {
      WiFi.begin(ssid, password);
      delay_ms(500);
    }
  }
}
void TaskNtp( void *pvParameters )
{
  ntpClient.begin();
  for (;;) {   
    bool updated = ntpClient.update();
    set_blinkt(ntp, updated);
    if(updated) {
      time_t utc = ntpClient.getEpochTime(); // might return bad value
      //bool ok = utc > 1583161881 / 2; // an approximate midpoint beterrn 1970 and 2020 to detect junk utc
      //set_blinkt(ntp, ok);
    //if (ok) {
      DS.set(utc);
      delay_min(10);
    }
    //delay_sec(10);


  }
}

void TaskUpdateDisplay(void *pvParameters)
{
  int8_t arr[] = { -1, -1, -1, -1};
  //int8_t sec = 0;
  //int8_t day = 1, hr = 14, min = 14, sec = 0;
  for (;;) {
    set_blinkt(ds, DS.chipPresent());
    time_t tim = DS.get();
    tim = myTZ.toLocal(tim, &tcr);
    arr[0] = day(tim);
    arr[2] = hour(tim);
    arr[3] = minute(tim);
    set_display(arr);

    delay_sec(1);
  }
}

void TaskAlive(void *pvParameters) 
{
  set_blinkt(alive, false);
  delay_sec(5);
  set_blinkt(alive, true);
  vTaskDelete(NULL);  
}
#define create_task(task) xTaskCreate(task, #task, 4096, 0, tskIDLE_PRIORITY, 0);
void setup()
{
  Serial.begin(115200);
  init_display();
  init_blinkt();
  

  create_task(TaskAlive);
  create_task(TaskWifi);
  create_task(TaskUpdateDisplay);
  create_task(TaskNtp);
}

void loop() { }

// format and print a time_t value, with a time zone appended.
void printDateTime(time_t t, const char *tz)
{
  char buf[32];
  char m[4];    // temporary storage for month string (DateStrings.cpp uses shared buffer)
  strcpy(m, monthShortStr(month(t)));
  sprintf(buf, "%.2d:%.2d:%.2d %s %.2d %s %d %s",
          hour(t), minute(t), second(t), dayShortStr(weekday(t)), day(t), m, year(t), tz);
  Serial.println(buf);
}
