#include "BritepadShared.h"
#include "Clock.h"
#include "Sound.h"

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
    chimeTimer.setMillis(1000, (timerCallback_t)chimeCallback, (void*)this);
    sound.beep();
    chimesRemaining--;
  }
}

void Clock::adjust(long adjustment) {
  adjustTime(adjustment);
  set = true;
  resetChime();
}


Clock::Clock(void) {
  // set clock to a recent time
  setTime(4,20,0,1,1,2015);
}

