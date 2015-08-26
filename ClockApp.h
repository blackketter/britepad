#ifndef _ClockApp_
#define _ClockApp_

#include "BPApp.h"
#include "Britepad.h"

class ClockApp : public BPApp {
  public:
    ClockApp(void) : BPApp() {};
    BPApp* run(void);
    bool isScreensaver(void) { return true; };
    const char* name(void) { return "Clock"; };
  private:
    int currentColor = screen.red;

};

#endif

