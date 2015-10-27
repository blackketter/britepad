#ifndef _TimerApp_
#define _TimerApp_

#include "ScreensaverApp.h"
#include "BritepadShared.h"
#include "Timer.h"
#include "StopwatchApp.h"

class TimerApp : public StopwatchApp {
  public:
    void begin(AppMode asMode) { StopwatchApp::begin(asMode); beeps = 10; }  // always start as a screensaver
    void end(BritepadApp* nextApp);
    bool wantsToBeScreensaver(void) { return isRunning() || !isReset(); }
    bool disablesScreensavers(void) { return false; }
    const char* name(void) { return "Timer"; };

    appid_t id() { return ID; };
    static constexpr appid_t ID = "timr";

    void setTime(time_t t);
    void alarm(void);

  private:
    time_t timerTime(void) { return timer_dur; };

    virtual bool isPaused() { return mytimer.isPaused(); };
    virtual bool isRunning(void) { return mytimer.isRunning(); }  // are we counting down?
    virtual bool isReset(void) { return (mytimer.isReset() || (isPaused()  &&  (mytimer.remainingSecs() == timerTime()))); }

    virtual void pause(void) { mytimer.pause(); }
    virtual void resume(void) { mytimer.resume(); }
    virtual void reset(void);

    virtual void drawTime(void);

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


