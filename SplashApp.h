#ifndef _SplashApp_
#define _SplashApp_

#include "ScreensaverApp.h"

class SplashApp : public ScreensaverApp {
  public:
    void begin(void);
    BritepadApp* run(void);
    const char* name(void) { return "Splash"; };
    bool displaysStatusBar(void) { return false; };
  private:
    long drawindex = 0;
    color_t currColor;
    millis_t firstRun;
    static const millis_t splashDuration = 1000*5;
};

#endif

