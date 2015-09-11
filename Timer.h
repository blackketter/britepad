#ifndef _Timer_
#define _Timer_

#include "Types.h"

// return true if you want to cancel the timer callback
typedef bool (*timerCallback_t)(void*);


class Timer {
  public:
    Timer(void) {};

    void setSecs(time_t setTime, timerCallback_t callback = nil, void* callbackData = nil, bool repeat = false);
    void setMillis(millis_t millisDur, timerCallback_t callback = nil, void* callbackData = nil, bool repeat = false);

    void setClockTime(time_t clockTimeSet, timerCallback_t callback = nil, void* callbackData = nil);

    time_t remainingSecs(void);  // seconds from now (works on both kinds of timer)
    long remainingMillis(void);  // millis from now (works on both kinds of timer)

    time_t timeInSecs(void);   // clock time of timer (works on both kinds of timer)
    long timeInMillis(void);   // millis() time of timer (works on both kinds of timer)

    time_t durationSecs(void);  // how far out was the timer when it was initially set?
    long durationMillis(void);

    bool passed(void);         // is the timer in the past

    void cancel(void);         // cancel timer including callback
    void pause(void);          // pause timer
    void resume(void);         // resume timer
    bool isPaused(void);       // is paused

    static void idle(void);    // idle so callbacks get a chance to run

  private:
    Timer* next = nil;
    Timer* prev = nil;

    time_t clockTime = 0;

    long millisTime = 0;
    unsigned long millisDur = 0;
    bool repeatTimer;

    timerCallback_t cb = nil;
    void* cbd = nil;

    void insert(timerCallback_t callback, void* callbackData);
    void remove(void);

    static Timer* first;

};

#endif

