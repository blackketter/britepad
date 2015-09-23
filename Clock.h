#ifndef _Clock_
#define _Clock_

#include "Timer.h"

class Clock {
  public:
    Clock(void);
    void chimerCallback(void);
    void beginSetClock(void) { setting = true; chimeTimer.cancel(); };
    void endSetClock(void) { setting = false; resetChime(); } ;
    void adjust(long adjustment);
    bool hasBeenSet() { return set && !setting; }
    void shortTime(char * timeStr);

    millis_t millis();  // remember, millis_t is now signed 64-bits
    time_t now();

    bool isAM();
    uint8_t hourFormat12();
    uint8_t minute();
    uint8_t second();

  private:
    void resetChime(void);
    Timer chimeTimer;
    int chimesRemaining = 0;
    bool set = false;
    bool setting = false;
    static const millis_t chimeInterval = 500;
    millis_t lastMillis;
    millis_t millisOffset = 0;
};

extern Clock clock;

#endif
