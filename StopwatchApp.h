#ifndef _StopwatchApp_
#define _StopwatchApp_

#include "BritepadApp.h"
#include "Button.h"

class StopwatchApp : public BritepadApp {
  public:
    BritepadApp* run(void);
    void begin(AppMode asMode);
    const char* name(void) { return "Stopwatch"; };
    bool displaysStatusBar(void) { return true; }
    bool wantsToBeScreensaver(void) { return isStopwatchRunning(); }
    bool disablesScreensavers(void) { return isAppMode(INTERACTIVE); }

    appid_t id() { return ID; };
    static constexpr appid_t ID = "stpw";

  private:
    void redrawButtons();
    bool isStopwatchRunning() { return startMillis > 0; };
    long startMillis = -1;  // start out paused at zero(ish)
    millis_t lastDrawMillis;
    RoundButton pauseButton;
    RoundButton resetButton;
};

#endif


