#include "RTClib.h"
#include <zeroseg.h> // one of my fantabulous libraries
#include <Timezone.h> // https://github.com/JChristensen/Timezone

#include <debounce.h> // a project here that does falling buttons

typedef unsigned long ulong;
//typedef unsigned long micros_t;
typedef ulong ms_t;



///////////////////////////////////////////////////////////
// DS3231


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


typedef void (*sm_func_t)(int); // state machine pointer

//void sm_nada(int e) {}

void sm_default(int ev);
sm_func_t sm_func = &sm_default;


// COMMON COMPONENTS end
///////////////////////////////////////////////////////////
// BUZZER

#define BZR 8

u32 buzzer_started = 0;
bool buzzer_enabled = false;

void buzzer_poll() {
  if (!buzzer_enabled) return;
  u32 elapsed = millis() - buzzer_started;
  ulong segment = elapsed % 5000;
  bool on = segment < 250 || ( 500 < segment && segment < 750);
  //Serial.print("sound ");
  if (on) {
    tone(BZR, 2525);// quindar
    //Serial.println("on");
  } else {
    noTone(BZR);
    digitalWrite(BZR, LOW);
    //Serial.println("off");
  }
}


void buzzer_start() {
  if(buzzer_enabled) return; // don't restart an already-running buzzer
  pinMode(BZR, OUTPUT);
  buzzer_started = millis();
  buzzer_enabled = true;
  buzzer_poll();
}

void buzzer_stop() {
  buzzer_enabled = false;
}

///////////////////////////////////////////////////////////

enum state_t {st_normal, st_adjusting, st_timing};

state_t state = st_normal;

enum {ev_poll, ev_blue_falling};

Debounce blue(3); // 0seg left button is A0, right button is A2
Debounce sw_left(A0);
Debounce sw_right(A2);
//FallingButton sw_adj(2); // adjust time up or down

//ezButton sw0(3);

#define SW_ADJ 2



void setup() {
  init_7219();
  rtc.begin();
  Serial.begin(115200);
  //update_regular_display();
  //sm_func(666); // test out the function

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

ulong om_start_time;

void sm_om_timing(int ev) {
  ulong elapsed_secs = (millis() - om_start_time) / 1000;
  switch (ev) {
    case ev_blue_falling:
      buzzer_stop();
      sm_func = sm_default;
      break;
    case ev_poll:
      update_counter_display(elapsed_secs);
      if (elapsed_secs > 60UL * 30UL) {
        buzzer_start();
      }
  }
}

void sm_om_starting(int ev) {
  if (ev != ev_poll) return;
  om_start_time = millis();
  sm_func = sm_om_timing;
}

void sm_default(int ev) {
  switch (ev) {
    case ev_blue_falling:
      sm_func = sm_om_starting;
      break;
    case ev_poll:
    default:
      update_regular_display();
  }
}

void loop() {
  buzzer_poll();

  if (blue.falling()) {
    sm_func(ev_blue_falling);
  } else {
    sm_func(ev_poll);
  }
  delay(1);
}
