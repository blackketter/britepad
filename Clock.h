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

    millis_t millis();
    time_t now();

    uint8_t hourFormat12();
    bool isAM();
    uint8_t minute();

  private:
    void resetChime(void);
    Timer chimeTimer;
    int chimesRemaining = 0;
    bool set = false;
    bool setting = false;
    static const millis_t chimeInterval = 500;
};

extern Clock clock;

#endif
