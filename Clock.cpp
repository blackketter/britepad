#include <Time.h>
#include "BritepadShared.h"
#include "Clock.h"
#include "Sound.h"
#include "Debug.h"

// todo - deal with 32bit millis wraparound (every 12 days!)

time_t getRTCTime()
{
  return Teensy3Clock.get();
}

Clock::Clock(void) {
    // todo - if the clock has NEVER been set, then set it to a reasonable time

    setSyncProvider(getRTCTime);
    if (timeStatus()!= timeSet || year() < 2015) {
//      DEBUG_LN("Unable to sync with the RTC");
      // set clock to a recent time - not needed with RTC
      ::setTime(16,20,0,1,1,2015);
    } else {
//      DEBUG_LN("RTC has set the system time");
      set = true;
    }
}



void chimeCallback(void* data) {
  ((Clock*)data)->chimerCallback();
}

void Clock::resetChime(void) {
  DEBUG_LN("Calling reset chime");
  if (hasBeenSet()) {
    DEBUG_PARAM_LN("resetting chime timer time", hourFormat12());
    tmElements_t chimeTime;
    breakTime(now(), chimeTime);
    chimeTime.Minute = 0;
    chimeTime.Second = 0;
    chimeTime.Hour++;
    chimeTimer.setClockTime(makeTime(chimeTime), (timerCallback_t)chimeCallback, (void*)this);

    // how many chimes at the next hour
    chimesRemaining = hourFormat12() + 1;
    if (chimesRemaining == 13) { chimesRemaining = 1; }
  }
}


void Clock::chimerCallback(void) {
  if (chimesRemaining == 0) {
    resetChime();
  } else {
    chimeTimer.setMillis(chimeInterval, (timerCallback_t)chimeCallback, (void*)this);
    sound.beep();
    chimesRemaining--;
  }
}

void Clock::adjust(stime_t adjustment) {
  DEBUG_PARAM_LN("before:", ::now());
  Teensy3Clock.set(now() + adjustment);

  // force a resync
  setSyncProvider(getRTCTime);

  set = true;

  resetChime();
  DEBUG_PARAM_LN("after:", ::now());
}


millis_t Clock::millis() {
  millis_t nowMillis = ::millis();
  if (nowMillis < lastMillis) {
    // millis have rolled over, we add a new offset
    millisOffset += 0x0000000100000000;
  }
  lastMillis = nowMillis;
  return nowMillis + millisOffset;
}

time_t Clock::now() {
  return ::now();
}

uint8_t Clock::hourFormat12() {
  return ::hourFormat12();
}

uint8_t Clock::minute() {
  return ::minute();
}

uint8_t Clock::second() {
  return ::second();
}

uint16_t Clock::year() {
  return ::year();
}

uint8_t Clock::month() {
  return ::month();
}

uint8_t Clock::day() {
  return ::day();
}

uint8_t Clock::weekday() {
  return ::weekday();
}

bool Clock::isAM() {
  return ::isAM();
}

void Clock::shortTime(char * timeStr) {
  sprintf(timeStr, "%d:%02d %s", hourFormat12(), minute(), isAM() ? "am":"pm");
};

static const uint8_t monthDays[]={31,28,31,30,31,30,31,31,30,31,30,31}; // API starts months from 1, this array starts from 0
static const char* dayStrings[] = { "", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday" };
static const char* monthStrings[] = { "", "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };

uint8_t Clock::daysInMonth(uint8_t m) {
  // todo: make this work for leap years
  return monthDays[m-1]; // m is 1 based, the array is zero based
}

void Clock::longDate(char* dateStr) {
  sprintf(dateStr, "%s, %s %d, %d", dayStrings[weekday()], monthStrings[month()], day(), year());
}

void Clock::shortDate(char* dateStr) {
  sprintf(dateStr, "%d-%02d-%02d", year(), month(), day());
}
