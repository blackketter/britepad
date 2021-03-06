#ifndef _CountdownApp_
#define _CountdownApp_

#include "ScreensaverApp.h"
#include "BritepadShared.h"
#include "Timer.h"
#include "SetCountdownApp.h"

class CountdownApp : public ScreensaverApp {
  public:
    void begin();
    void run();
    void setAppMode(AppMode asMode);

    virtual bool canBeInteractive() { return true; }
    virtual bool displaysStatusBar() { return true; }

    const char* name() { return "T-Minus"; };
    appid_t id() { return ID; };
    static constexpr appid_t ID = "cntd";
    AppType getAppType() { return (AppType)(SCREENSAVER_APP | SETTINGS_APP); }

    time_t getTime() { return countdownTime.getSeconds(); }
    void setTime(time_t newTime);

  protected:
    void redraw();
    // todo: allocate this when needed, free it when it exits
    SetCountdownApp _setCountdownApp;
    RoundButton setButton;
    const char* countdownTimePrefStr = "cntt";

    Time countdownTime;
    millis_t lastDrawMillis = 0;
};

#endif


