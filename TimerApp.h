#ifndef _TimerApp_
#define _TimerApp_

#include "ScreensaverApp.h"
#include "BritepadShared.h"
#include "Timer.h"
#include "StopwatchApp.h"

class TimerApp : public StopwatchApp {
  public:
    void begin(AppMode asMode) { StopwatchApp::begin(SCREENSAVER); beeps = 10; }  // always start as a screensaver
    void end(BritepadApp* nextApp);
    bool wantsToBeScreensaver(void) { return !isReset(); }
    bool disablesScreensavers(void) { return false; }
    const char* name(void) { return "Timer"; };

    void setTime(time_t t);
    time_t timerTime(void) { return timer_dur; };
    bool timerActive(void) { return mytimer.running(); }  // are we counting down?
    void alarm(void);

    appid_t id() { return ID; };
    static constexpr appid_t ID = "timr";

    virtual bool isPaused() { return mytimer.isPaused(); };
    virtual void pause(void) { mytimer.pause(); }
    virtual void resume(void) { mytimer.resume(); }
    virtual void reset(void);
    virtual bool isReset(void) { return !mytimer.running(); }
    virtual void drawTime(void);

  private:
    const int alarm_dur = (60);  // show the time for a while after the alarm goes off

    coord_t last_width;
    Timer mytimer;
    int beeps;
    time_t timer_dur = 0;
    time_t alarm_sounded = 0;
};

#endif


