#ifndef _Clock_
#define _Clock_

#include <Time.h>
#include "Timer.h"

class Clock {
  public:
    Clock(void);
    void chimerCallback(void);
    void beginSetClock(void) { setting = true; chimeTimer.cancel(); };
    void endSetClock(void) { setting = false; resetChime(); } ;
    void adjust(long adjustment);
    bool hasBeenSet() { return set && !setting; }

  private:
    void resetChime(void);
    Timer chimeTimer;
    int chimesRemaining = 0;
    bool set = false;
    bool setting = false;
    static const millis_t chimeInterval = 500;
};

#endif
