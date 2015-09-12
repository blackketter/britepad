#ifndef _StopwatchApp_
#define _StopwatchApp_

#include "BritepadApp.h"
#include "Britepad.h"

class StopwatchApp : public BritepadApp {
  public:
    BritepadApp* run(void);
    void begin(void) { BritepadApp::begin(); startMillis = lastDrawMillis = millis(); }
    const char* name(void) { return "Stopwatch"; };
    bool disablesScreensavers(void) { return true; }

    appid_t id() { return ID; }
    static constexpr appid_t ID = "stpw";

  private:
    millis_t startMillis;
    millis_t lastDrawMillis;
    color_t currentColor = screen.red;
};

#endif


