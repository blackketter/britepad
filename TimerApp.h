#ifndef _TimerApp_
#define _TimerApp_

#include "ScreensaverApp.h"
#include "Britepad.h"
#include "Time.h"

class TimerApp : public ScreensaverApp {
  public:
    BritepadApp* run(void);
    const char* name(void) { return "Timer"; };
    void setTime(time_t t) { timer_time = now() + t; };
    time_t timerTime(void) { return timer_time; };
    bool  timerActive(void) { return timer_time > now(); }
  private:
    const int display_zeros = 10; // (60*5);

    time_t timer_time;
    time_t last_time;
    color_t current_color = screen.red;

};

extern TimerApp* timerApp;

#endif


