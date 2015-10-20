#ifndef _Clock_
#define _Clock_

#include "Timer.h"

class Clock {
  public:
    Clock(void);
    void adjust(stime_t adjustment); // signed time
    bool hasBeenSet() { return set && !setting; }

    void beginSetClock(void) { setting = true; chimeTimer.cancel(); };
    void endSetClock(void) { setting = false; resetChime(); } ;

    void chimerCallback(void);

    void shortTime(char* timeStr);
    void longDate(char* dateStr);
    void shortDate(char* dateStr);

    millis_t millis();  // remember, millis_t is now signed 64-bits
    time_t now();

    bool isAM();
    uint8_t hourFormat12();
    uint8_t minute();
    uint8_t second();
    uint16_t year();
    uint8_t month();
    uint8_t day();
    uint8_t weekday();

    uint8_t daysInMonth(uint8_t m);  // months 1 based
    const time_t secsPerMin = 60;
    const time_t secsPerHour = 60*60;
    const time_t secsPerDay = 60*60*24;
    const time_t secsPerYear = 60*60*24*365;

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
