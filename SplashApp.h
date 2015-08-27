#ifndef _SplashApp_
#define _SplashApp_

#include "BritepadApp.h"
#include "Britepad.h"

class SplashApp : public BritepadApp {
  public:
    SplashApp(void) : BritepadApp() {};
    void begin(void);
    BritepadApp* run(void);
    bool isScreensaver(void) { return true; };
    const char* name(void) { return "Splash"; };
};

#endif

