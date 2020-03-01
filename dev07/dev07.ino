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

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "uk.pool.ntp.org");


TimeChangeRule myBST = {"BST", Last, Sun, Mar, 1, 60};
TimeChangeRule mySTD = {"GMT", Last, Sun, Nov, 2, 0};
Timezone myTZ(myBST, mySTD);
TimeChangeRule *tcr;        //pointer to the time change rule, use to get TZ abbrev

void TaskUpdateRTC( void *pvParameters )
{
  for (;;) {
    time_t utc = timeClient.getEpochTime();
    bool ntp_good_update = timeClient.update();
    set_blinkt(1, ntp_good_update);
    if (ntp_good_update) { RTC.set(utc);}

    vTaskDelay(10000 / portTICK_PERIOD_MS);
  }
}

void TaskUpdateDisplay(void *pvParameters)
{
  int8_t arr[] = { -1, -1, -1, -1};
  //int8_t sec = 0;
  //int8_t day = 1, hr = 14, min = 14, sec = 0;
  for (;;) {
    set_blinkt(2, RTC.chipPresent());
    time_t tim = RTC.get();
    tim = myTZ.toLocal(tim, &tcr);
    arr[0] = day(tim);
    arr[2] = hour(tim);
    arr[3] = minute(tim);
    set_display(arr);

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
void setup()
{
  Serial.begin(115200);
  init_display();
  init_blinkt();

  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }
  timeClient.begin();

  setSyncProvider(RTC.get);   // the function to get the time from the RTC
  if (timeStatus() != timeSet)
    Serial.println("Unable to sync with the RTC");
  else
    Serial.println("RTC has set the system time");

  xTaskCreate(TaskUpdateDisplay, "TaskUpdateDisplay", 4096, 0, tskIDLE_PRIORITY, 0);
  xTaskCreate(TaskUpdateRTC, "TaskUpdateRTC", 4096, 0, tskIDLE_PRIORITY, 0);
}

void loop()
{


}

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
