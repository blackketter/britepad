#ifndef _LAUNCHERAPP_
#define _LAUNCHERAPP_

#include "BPApp.h"
#include "Britepad.h"

class LauncherApp : public BPApp {
  public:
    LauncherApp(void) : BPApp() {};
    void begin(void);
    void end(void);
    void run(void);
    bool isScreensaver(void) { return false; };
    const char* name(void) { return "Launcher"; };
  private:
};

#endif

