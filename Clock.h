#ifndef _Clock_
#define _Clock_

#include "Timer.h"
#include "Debug.h"

class Time {
  public:
    Time() {};
    virtual time_t get() { return curTime; };

    virtual void set(time_t newTime) { curTime = newTime; }
    virtual void adjust(stime_t adjustment) { curTime += adjustment; } // signed time
    virtual bool isTime(time_t newTime) { return newTime == curTime; }

    virtual void beginSetTime() {};
    virtual void endSetTime() {};

    void shortTime(char* timeStr);
    void longTime(char* timeStr);
    void longDate(char* dateStr);
    void shortDate(char* dateStr);

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

  protected:
    time_t curTime = 0;

};

// can only be set to a time of day, i.e. [0, secsPerDay)
class DayTime : public Time {
  public:
    virtual void set(time_t newTime) {  Time::set(newTime % secsPerDay); }
    virtual void adjust(stime_t adjustment)  { if (adjustment < 0) { adjustment = secsPerDay-((-adjustment)%secsPerDay); } set(adjustment+curTime); };
    virtual bool isTime(time_t newTime) { return curTime == (newTime % secsPerDay); }
    virtual time_t nextOccurance();
};

class Clock : public Time {

  public:
    Clock(void);
    millis_t millis();  // remember, millis_t is now signed 64-bits
    time_t now();

    virtual void set(time_t newTime);
    virtual time_t get() { return now(); };
    virtual void adjust(stime_t adjustment); // signed time

    virtual bool hasBeenSet() { return doneSet && !setting; }
    virtual void beginSetTime(void) { setting = true; chimeTimer.cancel(); };
    virtual void endSetTime(void) { setting = false; resetChime(); } ;

    void chimerCallback(void);

  private:

    void resetChime(void);
    Timer chimeTimer;
    int chimesRemaining = 0;
    static const millis_t chimeInterval = 500;
    bool doneSet = false;
    bool setting = false;
    millis_t lastMillis;
    millis_t millisOffset = 0;
};

extern Clock clock;

#endif
