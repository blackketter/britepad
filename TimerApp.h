#ifndef _TimerApp_
#define _TimerApp_

#include "ScreensaverApp.h"
#include "BritepadShared.h"
#include "Timer.h"
#include "StopwatchApp.h"

class TimerApp : public StopwatchApp {
  public:
    void begin() { StopwatchApp::begin(); beeps = 10; }  // always start as a screensaver
    void end();

    bool wantsToBeScreensaver() { return isRunning() || !isReset(); }
    bool disablesScreensavers() { return false; }

    const char* name() { return "Timer"; };
    appid_t id() { return ID; };
    static constexpr appid_t ID = "timr";

    void setTime(time_t t);
    time_t getTime() { return timer_dur; }
    time_t getTimeRemaining() { return mytimer.remainingSecs(); }
    void alarm();
    virtual bool isRunning() { return mytimer.isRunning(); }  // are we counting down?

  private:
    time_t timerTime() { return timer_dur; };

    virtual bool isPaused() { return mytimer.isPaused(); };
    virtual bool isReset() { return (mytimer.isReset() || (isPaused()  &&  (mytimer.remainingSecs() == timerTime()))); }

    virtual void pause() { mytimer.pause(); }
    virtual void resume() { mytimer.resume(); }
    virtual void reset();

    virtual void drawTime();

    const time_t alarmDur = 60;  // show the time for a while after the alarm goes off
    const int totalBeeps = 10;
    const millis_t beepInterval = 500;  // in millis
    const millis_t redrawInterval = 500;

    coord_t last_width;
    Timer mytimer;
    int beeps;
    time_t timer_dur = 0;
    time_t alarm_sounded = 0;
};

#endif


