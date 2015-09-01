#ifndef _SplashApp_
#define _SplashApp_

#include "ScreensaverApp.h"

class SplashApp : public ScreensaverApp {
  public:
    void begin(void);
    BritepadApp* run(void);
    bool isScreensaver(void) { return false; };
    const char* name(void) { return "Splash"; };
};

#endif

