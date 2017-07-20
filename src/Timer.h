#ifndef _Timer_
#define _Timer_

#include "Arduino.h"
#include "Types.h"
#include "Clock.h"

typedef void (*timerCallback_t)(void*);

class Timer {
  public:
    Timer() {};
    ~Timer();

    void setSecs(time_t setTime, timerCallback_t callback = nullptr, void* callbackData = nullptr, bool repeat = false);
    void setMillis(millis_t millisDur, timerCallback_t callback = nullptr, void* callbackData = nullptr, bool repeat = false);

    void setClockTime(time_t clockTimeSet, timerCallback_t callback = nullptr, void* callbackData = nullptr);

    time_t remainingSecs();  // seconds from now (works on both kinds of timer)
    millis_t remainingMillis();  // millis from now (works on both kinds of timer)

    time_t timeInSecs();   // clock time of timer (works on both kinds of timer)
    millis_t timeInMillis();   // millis() time of timer (works on both kinds of timer)

    time_t durationSecs();  // how far out was the timer when it was initially set?
    millis_t durationMillis();


    void cancel();         // cancel timer including callback

    void pause();          // pause timer
    void resume();         // resume timer
    bool isPaused();       // is paused
    bool isRunning();        // is the timer running?  (i.e. is not paused and is set)
    bool hasPassed();         // is the timer in the past
    bool isReset() { return ((_millisTime == 0) && (_clockTime == 0)); }

    static void idle();    // idle so callbacks get a chance to run
    static Timer* first() { return _first; }
    void* data() { return _cbd; }
    Timer* next() { return _next; }

  protected:
    void insert(timerCallback_t callback, void* callbackData);
    void remove();

    Timer* _next = nullptr;
    Timer* _prev = nullptr;

    time_t _clockTime = 0;

    millis_t _millisTime = 0;
    millis_t _millisDur = 0;
    bool _repeatTimer;

    timerCallback_t _cb = nullptr;
    void* _cbd = nullptr;

    static Timer* _first;

};

#endif

