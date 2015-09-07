#include <Arduino.h>
#include <Time.h>
#include "Timer.h"
#include "Debug.h"

// todo - deal with 32bit millis wraparound (every 12 days!)

Timer* Timer::first = nil;

void Timer::setClockTime(time_t clockTimeSet, timerCallback_t callback, void* callbackData) {
  cancel();
  insert(callback, callbackData);
  clockTime = clockTimeSet;
  millisDur = (clockTimeSet-now())*1000;
}

void Timer::setSecs(time_t secs, timerCallback_t callback, void* callbackData, bool repeat) {
  setMillis(secs*1000, callback, callbackData, repeat);
}

void Timer::setMillis(millis_t millisDuration, timerCallback_t callback, void* callbackData, bool repeat) {
  cancel();
  clockTime = 0;
  insert(callback, callbackData);
  repeatTimer = repeat;
  millisDur = millisDuration;
  millisTime = millis() + millisDur;
}

long Timer::remainingMillis(void) {
  long remainingMillis = 0;
  if (millisTime) {
    if (isPaused()) {
      return -millisTime;
    } else {
      return (millisTime - millis());
    }
  } else if (clockTime) {
    // todo make this more accurate using the difference between millis() and now()
    return remainingSecs() * 1000;
  }
  return remainingMillis;
}

time_t Timer::remainingSecs(void) {
  long remainingSecs = 0;
  if (millisTime) {
    return remainingMillis()/1000;
  } else if (clockTime) {
    if (isPaused()) {
      return -clockTime;
    } else {
      return (clockTime - now());
    }
  }
  return remainingSecs;
}

time_t Timer::timeInSecs(void) {
  return now()+remainingSecs();
}

long Timer::timeInMillis(void) {
  return millis()+remainingMillis();
}

time_t Timer::durationSecs(void) {
  if (millisDur) {
    return millisDur/1000;
  } else {
    return 0;
  }
}
long Timer::durationMillis(void) {
  if (millisDur) {
    return millisDur;
  } else {
    return 0;
  }
}

bool Timer::passed(void) {

  if (isPaused()) { return false; }

  if (millisTime) {
    if (millis() >= (unsigned long)millisTime) {
      return true;
    }
  } else if (clockTime) {
    if (now() >= clockTime) {
      return true;
    }
  }
  return false;
}

void Timer::cancel(void) {
  millisTime = 0;
  clockTime = 0;
  millisDur = 0;
  repeatTimer = 0;
  remove();
}

void Timer::pause(void) {

  if (isPaused()) {
    return;
  }

  if (millisTime) {
    millisTime = -(millisTime - millis());
  } else if (clockTime) {
    clockTime = -(clockTime - now());
  }
}

void Timer::resume(void) {
  if (!isPaused()) {
    return;
  }
  if (millisTime) {
    millisTime = millis() - millisTime;
  } else if (clockTime) {
    clockTime = now() - clockTime;
  }
}

bool Timer::isPaused(void) {
  return (millisTime < 0) || (clockTime < 0);
}

void Timer::idle(void) {
  Timer* t = first;

  while (t) {
    if (t->passed()) {

      // save callback data so we can call the timer after it's been removed.
      // this way the callback function can reuse the timer.
      timerCallback_t callback = t->cb;
      void* callbackdata = t->cbd;

      if (t->repeatTimer) {
        t->millisTime += t->millisDur;
        t = t->next;
      } else {
        Timer* nextup = t->next;
        t->remove();
        t = nextup;
      }

      (callback)(callbackdata);

    } else {
      t = t->next;
    }
  }
}

void Timer::insert(timerCallback_t callback, void* callbackData) {
  remove();
  if (callback) {
    next = first;
    prev = nil;
    if (first) {
      first->prev = this;
    }
    first = this;
    cb = callback;
    cbd = callbackData;
  }
}

void Timer::remove(void) {
  if (cb) {
    Timer* t = first;

    while (t != nil) {
      if (t == this) {
        if (this->prev) {
          prev->next = next;
        } else {
          first = next;
        }
        if (this->next) {
          next->prev = prev;
        }
        t = nil;
      } else {
        t = t->next;
      }
    }

    prev = nil;
    next = nil;
    cb = nil;
    cbd = nil;
    repeatTimer = 0;
  }
}
