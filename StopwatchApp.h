#ifndef _StopwatchApp_
#define _StopwatchApp_

#include "BritepadApp.h"
#include "Britepad.h"
#include "Time.h"

class StopwatchApp : public BritepadApp {
  public:
    BritepadApp* run(void);
    void begin(void) { BritepadApp::begin(); startMillis = lastDrawMillis = millis(); }
    const char* name(void) { return "Stopwatch"; };
    bool disablesScreensavers(void) { return true; }
  private:
    long startMillis;
    long lastDrawMillis;
    color_t currentColor = screen.red;
};

#endif


