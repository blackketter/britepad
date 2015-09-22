#ifndef _TimerApp_
#define _TimerApp_

#include "ScreensaverApp.h"
#include "BritepadShared.h"
#include "Timer.h"

class TimerApp : public ScreensaverApp {
  public:
    BritepadApp* run(void);
    void begin(void) { ScreensaverApp::begin(); beeps = 10; }
    void end(BritepadApp* nextApp);
    const char* name(void) { return "Timer"; };
    void setTime(time_t t);
    time_t timerTime(void) { return timer_dur; };

    bool timerActive(void) { return mytimer.running(); }  // are we counting down?

    bool wantsToBeScreensaver(void) { return running; }  // stay being screensaver if counting down or has just gone off

    bool getScreensaverEnabled(void) { return mytimer.running(); }  // switch to being screensaver if counting down
    bool displaysStatusBar(void) { return true; }

    void alarm(void);
    void cancel(void);

    appid_t id() { return ID; };
    static constexpr appid_t ID = "timr";

  private:
    const int alarm_dur = (60*5);

    time_t last_time_drawn = 0;
    color_t current_color = screen.red;
    coord_t last_width;
    Timer mytimer;
    int beeps;
    time_t timer_dur = 0;
    time_t alarm_sounded = 0;
    bool running = false;
};

#endif


