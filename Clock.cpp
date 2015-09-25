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
    if (timeStatus()!= timeSet) {
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

void Clock::adjust(long adjustment) {
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

bool Clock::isAM() {
  return ::isAM();
}

void Clock::shortTime(char * timeStr) {
  sprintf(timeStr, "%d:%02d %s", hourFormat12(), minute(), isAM() ? "am":"pm");
};
