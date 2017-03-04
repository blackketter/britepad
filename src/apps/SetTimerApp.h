#ifndef _SetTimerApp_
#define _SetTimerApp_

#include "BritepadApp.h"
#include "BritepadShared.h"
#include "TimerApp.h"

class SetTimerApp : public BritepadApp {

  public:
    SetTimerApp(int32_t pos);

    void init();
    void end();

    void begin(AppMode asMode);
    void run();

    color_t appButtonColor() { return screen.LightBlue; }

    bool disablesScreensavers() { return true; }

    const char* name() { return "Set Timer"; }
    appid_t id() { return ID; };
    static constexpr appid_t ID = "stmr";
    AppType getAppType() { return TIMER_APP; }

    int32_t getDuration() { return _customTimerDuration; }

  private:
    int32_t _customTimerDuration = 30*60;  // there's only one custom timer
    static const int _buttonCount = 5;
    RoundButton button[_buttonCount];

    void drawTime();
    void drawButtons();
};

extern SetTimerApp theSetTimerApp;
#endif

