#include "BritepadShared.h"
#include "Timer.h"
#include "Clock.h"

Timer* Timer::_first = nullptr;

Timer::~Timer() {
  remove();
};

// fixme: handle other clocks besides "clock"
void Timer::setClockTime(time_t clockTimeSet) {
  cancel();
  _clockTime = clockTimeSet;
  _millisDur = (clockTimeSet-clock.now())*1000;
  insert();
}

void Timer::setMillis(millis_t millisDuration, bool repeat) {
  cancel();
  _clockTime = 0;
  _repeatTimer = repeat;
  _millisDur = millisDuration;
  _millisTime = Uptime::millis() + _millisDur;
  insert();
}

void Timer::setSecs(time_t secs, bool repeat) {
  setMillis(secs*1000, repeat);
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
  return _millisDur/1000;
}

millis_t Timer::durationMillis() {
    return _millisDur;
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
      Timer* nextup = t->_next;
      t->remove();
      t->callback();
      if (t->_repeatTimer) {
        t->_millisTime += t->_millisDur;
        t->insert();
      }
      t = nextup;
    } else {
      // if we're past the last millis-based clock, then shortcut out
      if (t->_clockTime == 0) {
        break;
      }
      t = t->_next;
    }
  }
}

void Timer::insert() {

  if (_first == nullptr) {
    _next = nullptr;
    _prev = nullptr;
    _first = this;
    return;
  }

  Timer* t = _first;
  while (1) {
    if (_millisTime > t->_millisTime) {
      if (t->_next) {
        t = t->_next;
      } else {
        _next = nullptr;
        t->_next = this;
        return;
      }
    } else {
      _prev = t->_prev;
      if (_prev) {
        _prev->_next = this;
      } else {
        _first = this;
      }
      t->_prev = this;
      _next = t;
      return;
    }
  }
}

void Timer::remove() {

  // no timer
  if (!_first) {
    return;
  }

  // this timer isn't in the list
  if ((_prev == 0) && (_first != this)) {
    return;
  }

  // make the previous or head point to the next one
  if (_prev) {
    _prev->_next = _next;
  } else {
    _first = _next;
  }

  // make the next one (if there is one) point back to the old previous one
  if (_next) {
    _next->_prev = _prev;
  }

  _prev = nullptr;
  _next = nullptr;
}

////////////////////////////////////////////////////////////////////////////////
void CallbackTimer::setClockTime(time_t clockTimeSet, timerCallback_t callback, void* callbackData) {
  _cb = callback;
  setData(callbackData);
  Timer::setClockTime(clockTimeSet);
}

void CallbackTimer::setSecs(time_t secs, timerCallback_t callback, void* callbackData, bool repeat) {
  _cb = callback;
  setData(callbackData);
  Timer::setSecs(secs,repeat);
}

void CallbackTimer::setMillis(millis_t millisDuration, timerCallback_t callback, void* callbackData, bool repeat) {
  _cb = callback;
  setData(callbackData);
  Timer::setMillis(millisDuration,repeat);
}


