#include <debounce.h> // a project here that does falling buttons

/*  blue button sw0 turns timer on or off
    left button on zeroseg toggles between timer and clock display
*/

//#include <ezButton.h>

typedef unsigned long ulong;
//typedef unsigned long micros_t;
typedef ulong ms_t;


///////////////////////////////////////////////////////////
// DS3231

#include "RTClib.h"
#include <zeroseg.h> // one of my fantabulous libraries
#include <Timezone.h> // https://github.com/JChristensen/Timezone

TimeChangeRule myBST = {"BST", Last, Sun, Mar, 1, 60};
TimeChangeRule mySTD = {"GMT", Last, Sun, Oct, 2, 0};
Timezone myTZ(myBST, mySTD);
TimeChangeRule *tcr;        //pointer to the time change rule, use to get TZ abbrev

RTC_DS3231 rtc;
DateTime get_time() {
  // only call the RTC occasionally to prevent peculiar noise coming from module
  DateTime dt = rtc.now();
  /*
    static auto snap = millis();
    if (millis() - snap > 1000UL * 60UL) {
    Serial.println("Refreshing RTC");
    dt = rtc.now();
    snap = millis();
    }
    auto tim = dt.unixtime() + (millis() - snap) / 1000;

  */

  auto tim = dt.unixtime();
  tim = myTZ.toLocal(tim, &tcr);
  DateTime dt_local{tim};
  return dt_local;
}


// DS3231 end
///////////////////////////////////////////////////////////
// COMMON COMPONENTS




// COMMON COMPONENTS end
///////////////////////////////////////////////////////////

enum state_t {st_normal, st_adjusting, st_timing};

state_t state = st_normal;

FallingButton sw0(3); // 0seg left button is A0, right button is A2
FallingButton sw_left(A0);
FallingButton sw_right(A2);
//FallingButton sw_adj(2); // adjust time up or down

//ezButton sw0(3);

#define SW_ADJ 2

#define BZR 8
void setup() {
  init_7219();
  rtc.begin();
  Serial.begin(115200);
  update_regular_display();
  pinMode(BZR, OUTPUT);

  // see if I can get rid of the buzzing sound
  // doesn't seem to help though
  rtc.disable32K();
  rtc.disableAlarm(0);
  rtc.disableAlarm(1);
  rtc.disableAlarm(2);

  pinMode(SW_ADJ, INPUT_PULLUP);
}

void show_dec(int pos, int val, bool dp = false) {
  int v = val % 10;
  if (dp) v |= 1 << 7; // add in the decimal point
  transfer_7219(pos, v);
  transfer_7219(pos + 1, val / 10);
}

bool show_clock = true;

void update_regular_display() {
  DateTime dt = get_time();
  show_dec(1, dt.minute());
  show_dec(3, dt.hour(), true);
  transfer_7219(5, 0b1111); // blank
  transfer_7219(6, 0b1111); // blank
  show_dec(7, dt.day());
}

void update_counter_display(ulong elapsed) {
  show_dec(1, elapsed % 60);
  show_dec(3, elapsed / 60, true);
  for (int i = 5; i < 9; i++) {
    transfer_7219(i, 0b1111); // blank
  }
}

/*
  void update_display(ulong elapsed_secs) {
  if (show_clock) {
    update_regular_display();
  } else {
    update_counter_display(elapsed_secs);
  }
  }
*/


bool sw_adj_low = false;

void do_adjusting() {
  if (!sw_adj_low) {
    state = st_normal;
    return;
  }

  int delta = 0;
  if (sw_left.falling())
    delta = -1;
  if (sw_right.falling())
    delta = 1;

  if (delta != 0) {
    auto dt = rtc.now();
    dt = dt + TimeSpan(delta * 60);
    rtc.adjust(dt);
    update_regular_display();

  }
}

static ulong start_time;

void do_normal() {
  if (sw_adj_low) {
    state = st_adjusting;
    return;
  }

  if (sw0.falling()) {
    start_time = millis();
    show_clock = false;
    state = st_timing;
    return;
  }

  update_regular_display();
}


void do_timing() {
  if (sw0.falling()) {
    show_clock = true;
    sound(false);
    state = st_normal;

  }

  ulong elapsed = 0;
  elapsed = millis() - start_time;
  long over_time = elapsed - 30UL * 1000UL * 60UL;
  if (over_time > 0) {
    ulong segment = over_time % 5000;
    sound(segment < 250 || ( 500 < segment && segment < 750)); // double-beeping
  }

  update_counter_display(elapsed / 1000);


}

void loop() {

  sw_adj_low = 1 - digitalRead(SW_ADJ);
  //do_state_machine();

  switch (state) {
    case st_normal:
      do_normal();
      break;
    case st_adjusting:
      do_adjusting();
      break;
    case st_timing:
      do_timing();
      break;
  }


  /*
    if (sw_left.falling()) {
      show_clock = !show_clock;
    }
  */

}

void sound(bool on) {
  Serial.print("sound ");
  if (on) {
    tone(BZR, 2525);// quindar
    Serial.println("on");
  } else {
    noTone(BZR);
    digitalWrite(BZR, LOW);
    Serial.println("off");
  }
}
