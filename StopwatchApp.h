#ifndef _StopwatchApp_
#define _StopwatchApp_

#include "BritepadApp.h"
#include "Britepad.h"
#include "Button.h"

class StopwatchApp : public BritepadApp {
  public:
    BritepadApp* run(void);
    void begin(void);
    const char* name(void) { return "Stopwatch"; };
    bool disablesScreensavers(void) { return true; }
    bool displaysStatusBar(void) { return true; }

    appid_t id() { return ID; };
    static constexpr appid_t ID = "stpw";

  private:
    long startMillis = -1;  // start out paused at zero(ish)
    millis_t lastDrawMillis;
    RoundButton pauseButton;
    RoundButton resetButton;
};

#endif


