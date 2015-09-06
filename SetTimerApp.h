#ifndef _SetTimerApp_
#define _SetTimerApp_

#include "BritepadApp.h"
#include "BritepadShared.h"
#include "TimerApp.h"

class SetTimerApp : public BritepadApp {

  private:
    const char* name_str;
    time_t timer_duration = 0;
    color_t button_color = 0;
    static TimerApp timer_app;

  public:
    SetTimerApp(const char* name, time_t duration);

    BritepadApp* run(void) { timer_app.setTime(timer_duration); return &timer_app; };
    const char* name(void) { return name_str; };
};


#endif

