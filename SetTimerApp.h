#ifndef _SetTimerApp_
#define _SetTimerApp_

#include "BritepadApp.h"
#include "BritepadShared.h"
#include "TimerApp.h"

class SetTimerApp : public BritepadApp {

  public:
    SetTimerApp(const char* name, time_t duration);
    SetTimerApp();

    void begin(AppMode asMode);
    void run(void);
    const char* name(void) { return name_str; };

    bool disablesScreensavers(void) { return true; }

    appid_t id() { return ID; };
    static constexpr appid_t ID = "stmr";

  private:
    bool edit = true;
    const char* name_str = "Timer";
    int timer_duration = 0;

    void drawTime();
    void drawButtons();
    static TimerApp timer_app;
    static const int buttoncount = 5;

    RoundButton button[buttoncount];
};


#endif

