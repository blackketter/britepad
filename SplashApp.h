#ifndef _SplashApp_
#define _SplashApp_

#include "ScreensaverApp.h"

class SplashApp : public ScreensaverApp {
  public:
    void begin(void);
    BritepadApp* run(void);
    const char* name(void) { return "Splash"; };
    bool displaysStatusBar(void) { return false; };
};

#endif

