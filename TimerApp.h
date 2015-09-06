#ifndef _TimerApp_
#define _TimerApp_

#include "ScreensaverApp.h"
#include "BritepadShared.h"
#include "Time.h"
#include "Timer.h"

class TimerApp : public ScreensaverApp {
  public:
    BritepadApp* run(void);
    void begin(void) { ScreensaverApp::begin(); beeps = 10; }
    const char* name(void) { return "Timer"; };
    void setTime(time_t t);
    time_t timerTime(void) { return timer_dur; };

    bool timerActive(void) { return running; }
    bool wantsToBeScreensaver(void) { return running; }
    bool screensaverIsEnabled(void) { return running; }

    void alarm(void);
    void cancel(void);

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


