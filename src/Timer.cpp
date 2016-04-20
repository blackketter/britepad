#include "BritepadShared.h"
#include "Timer.h"
#include "Debug.h"
#include "Clock.h"

Timer* Timer::first = nullptr;

void Timer::setClockTime(time_t clockTimeSet, timerCallback_t callback, void* callbackData) {
  cancel();
  insert(callback, callbackData);
  clockTime = clockTimeSet;
  millisDur = (clockTimeSet-clock.now())*1000;
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
  millisTime = Uptime::millis() + millisDur;
}

millis_t Timer::remainingMillis() {
  millis_t remainingMillis = 0;
  if (millisTime) {
    if (isPaused()) {
      return -millisTime;
    } else {
      return (millisTime - Uptime::millis());
    }
  } else if (clockTime) {
    // todo make this more accurate using the difference between millis() and now()
    return remainingSecs() * 1000;
  }
  return remainingMillis;
}

time_t Timer::remainingSecs() {
  millis_t remainingSecs = 0;
  if (millisTime) {
    return remainingMillis()/1000;
  } else if (clockTime) {
    if (isPaused()) {
      return -clockTime;
    } else {
      return (clockTime - clock.now());
    }
  }
  return remainingSecs;
}

time_t Timer::timeInSecs() {
  return clock.now()+remainingSecs();
}

millis_t Timer::timeInMillis() {
  return Uptime::millis()+remainingMillis();
}

time_t Timer::durationSecs() {
  if (millisDur) {
    return millisDur/1000;
  } else {
    return 0;
  }
}
millis_t Timer::durationMillis() {
  if (millisDur) {
    return millisDur;
  } else {
    return 0;
  }
}

bool Timer::hasPassed() {

  if (isPaused()) { return false; }

  if (millisTime) {
    if (Uptime::millis() >= millisTime) {
      return true;
    }
  } else if (clockTime) {
    if (clock.now() >= clockTime) {
      return true;
    }
  }
  return false;
}

bool Timer::isRunning() {
  return (millisTime > 0) || (clockTime > 0);
}

void Timer::cancel() {
  millisTime = 0;
  clockTime = 0;
  millisDur = 0;
  repeatTimer = 0;
  remove();
}

void Timer::pause() {

  if (isPaused()) {
    return;
  }

  if (millisTime) {
    millisTime = -(millisTime - Uptime::millis());
  } else if (clockTime) {
    clockTime = -(clockTime - clock.now());
  }
}

void Timer::resume() {
  if (!isPaused()) {
    return;
  }
  if (millisTime) {
    millisTime = Uptime::millis() - millisTime;
  } else if (clockTime) {
    clockTime = clock.now() - clockTime;
  }
}

bool Timer::isPaused() {
  return (millisTime < 0) || (clockTime < 0);
}

void Timer::idle() {
  Timer* t = first;

  while (t) {
    if (t->hasPassed()) {

      // save callback data so we can call the timer after it's been removed.
      // this way the callback function can reuse the timer.
      timerCallback_t callback = t->cb;
      void* callbackdata = t->cbd;

      if (t->repeatTimer) {
        t->millisTime += t->millisDur;
        t = t->next;
      } else {
        Timer* nextup = t->next;
        t->cancel();  // remove the timer and reset its timing values
        t = nextup;
      }

      if (callback) {
        (callback)(callbackdata);
      }

    } else {
      t = t->next;
    }
  }
}

void Timer::insert(timerCallback_t callback, void* callbackData) {
  remove();
  if (callback) {
    next = first;
    prev = nullptr;
    if (first) {
      first->prev = this;
    }
    first = this;
    cb = callback;
    cbd = callbackData;
  }
}

void Timer::remove() {
  if (cb) {
    Timer* t = first;

    while (t != nullptr) {
      if (t == this) {
        if (this->prev) {
          prev->next = next;
        } else {
          first = next;
        }
        if (this->next) {
          next->prev = prev;
        }
        t = nullptr;
      } else {
        t = t->next;
      }
    }

    prev = nullptr;
    next = nullptr;
    cb = nullptr;
    cbd = nullptr;
    repeatTimer = 0;
  }
}