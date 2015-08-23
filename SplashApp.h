#ifndef _SplashApp_
#define _SplashApp_

#include "BPApp.h"
#include "Britepad.h"

class SplashApp : public BPApp {
  public:
    SplashApp(void) : BPApp() {};
    void begin(void);
    BPApp* run(void);
    bool isScreensaver(void) { return true; };
    const char* name(void) { return "Splash"; };
};

#endif

