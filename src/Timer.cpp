#include "BritepadShared.h"
#include "Timer.h"
#include "Clock.h"

Timer* Timer::_first = nullptr;

Timer::~Timer() {
  remove();
};

// fixme: handle other clocks besides "clock"
void Timer::setClockTime(time_t clockTimeSet, timerCallback_t callback, void* callbackData) {
  cancel();
  insert(callback, callbackData);
  _clockTime = clockTimeSet;
  _millisDur = (clockTimeSet-clock.now())*1000;
}

void Timer::setSecs(time_t secs, timerCallback_t callback, void* callbackData, bool repeat) {
  setMillis(secs*1000, callback, callbackData, repeat);
}

void Timer::setMillis(millis_t millisDuration, timerCallback_t callback, void* callbackData, bool repeat) {
  cancel();
  _clockTime = 0;
  insert(callback, callbackData);
  _repeatTimer = repeat;
  _millisDur = millisDuration;
  _millisTime = Uptime::millis() + _millisDur;
}

millis_t Timer::remainingMillis() {
  millis_t remainingMillis = 0;
  if (_millisTime) {
    if (isPaused()) {
      return -_millisTime;
    } else {
      return (_millisTime - Uptime::millis());
    }
  } else if (_clockTime) {
    // todo make this more accurate using the difference between millis() and now()
    return remainingSecs() * 1000;
  }
  return remainingMillis;
}

time_t Timer::remainingSecs() {
  millis_t remainingSecs = 0;
  if (_millisTime) {
    return remainingMillis()/1000;
  } else if (_clockTime) {
    if (isPaused()) {
      return -_clockTime;
    } else {
      return (_clockTime - clock.now());
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
  if (_millisDur) {
    return _millisDur/1000;
  } else {
    return 0;
  }
}
millis_t Timer::durationMillis() {
  if (_millisDur) {
    return _millisDur;
  } else {
    return 0;
  }
}

bool Timer::hasPassed() {

  if (isPaused()) { return false; }

  if (_millisTime) {
    if (Uptime::millis() >= _millisTime) {
      return true;
    }
  } else if (_clockTime) {
    if (clock.now() >= _clockTime) {
      return true;
    }
  }
  return false;
}

bool Timer::isRunning() {
  return (_millisTime > 0) || (_clockTime > 0);
}

void Timer::cancel() {
  _millisTime = 0;
  _clockTime = 0;
  _millisDur = 0;
  _repeatTimer = 0;
  remove();
}

void Timer::pause() {

  if (isPaused()) {
    return;
  }

  if (_millisTime) {
    _millisTime = -(_millisTime - Uptime::millis());
  } else if (_clockTime) {
    _clockTime = -(_clockTime - clock.now());
  }
}

void Timer::resume() {
  if (!isPaused()) {
    return;
  }
  if (_millisTime) {
    _millisTime = Uptime::millis() - _millisTime;
  } else if (_clockTime) {
    _clockTime = clock.now() - _clockTime;
  }
}

bool Timer::isPaused() {
  return (_millisTime < 0) || (_clockTime < 0);
}

void Timer::idle() {
  Timer* t = _first;

  while (t) {
    if (t->hasPassed()) {

      // save callback data so we can call the timer after it's been removed.
      // this way the callback function can reuse the timer.
      timerCallback_t callback = t->_cb;
      void* callbackdata = t->_cbd;

      if (t->_repeatTimer) {
        t->_millisTime += t->_millisDur;
        t = t->_next;
      } else {
        Timer* nextup = t->_next;
        t->remove();  // remove the timer and reset its timing values
        t = nextup;
      }

      if (callback) {
        (callback)(callbackdata);
      }

    } else {
      t = t->_next;
    }
  }
}

void Timer::insert(timerCallback_t callback, void* callbackData) {
  remove();
  if (callback) {
    _next = _first;
    _prev = nullptr;
    if (_first) {
      _first->_prev = this;
    }
    _first = this;
    _cb = callback;
    _cbd = callbackData;
  }
}

void Timer::remove() {
  if (_cb) {
    Timer* t = _first;

    while (t != nullptr) {
      if (t == this) {
        if (this->_prev) {
          _prev->_next = _next;
        } else {
          _first = _next;
        }
        if (this->_next) {
          _next->_prev = _prev;
        }
        t = nullptr;
      } else {
        t = t->_next;
      }
    }

    _prev = nullptr;
    _next = nullptr;
    _cb = nullptr;
    _cbd = nullptr;
  }
}
