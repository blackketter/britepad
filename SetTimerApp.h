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

  public:
    SetTimerApp(const char* name, time_t duration) { timer_duration = duration; name_str = name;  };

    BritepadApp* run(void) { timerApp->setTime(timer_duration); return timerApp; };
    const char* name(void) { return name_str; };
};


#endif

