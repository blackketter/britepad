#ifndef _SetTimerApp_
#define _SetTimerApp_

#include "BritepadApp.h"
#include "BritepadShared.h"
#include "TimerApp.h"

class SetTimerApp : public BritepadApp {

  public:
    SetTimerApp(const char* name, time_t duration, int32_t pos = defaultLauncherPosition);
    SetTimerApp();

    void begin(AppMode asMode);
    void run();
    const char* name() { return name_str; };

    virtual bool highlighted() { return !timer_app.isReset() && (timer_app.getTime() == timer_duration); }

    bool disablesScreensavers() { return true; }

    appid_t id() { return ID; };
    static constexpr appid_t ID = "stmr";
    AppType getAppType() { return TIMER_APP; }

    bool customTimerRunning();
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

