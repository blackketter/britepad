#ifndef _Timer_
#define _Timer_

#include "Arduino.h"
#include "Types.h"
#include "Clock.h"

// return true if you want to cancel the timer callback
typedef bool (*timerCallback_t)(void*);

class Timer {
  public:
    Timer() {};

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
    bool isReset() { return ((millisTime == 0) && (clockTime == 0)); }

    static void idle();    // idle so callbacks get a chance to run

  private:
    Timer* next = nullptr;
    Timer* prev = nullptr;

    time_t clockTime = 0;

    millis_t millisTime = 0;
    millis_t millisDur = 0;
    bool repeatTimer;

    timerCallback_t cb = nullptr;
    void* cbd = nullptr;

    void insert(timerCallback_t callback, void* callbackData);
    void remove();

    static Timer* first;

};

#endif

