#ifndef _SetTimerApp_
#define _SetTimerApp_

#include "BritepadApp.h"
#include "BritepadShared.h"
#include "TimerApp.h"

class SetTimerApp : public BritepadApp {

  public:
    SetTimerApp(const char* name, time_t duration);
    SetTimerApp();

    void begin();
    void run();
    const char* name() { return name_str; };

    virtual color_t buttonColor() { color_t c = BritepadApp::buttonColor(); return !timer_app.isReset() && (timer_app.getTime() == timer_duration) ? screen.brighten(c,128) : c; }

    bool disablesScreensavers() { return true; }

    appid_t id() { return ID; };
    static constexpr appid_t ID = "stmr";

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

