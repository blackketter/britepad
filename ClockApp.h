#ifndef _ClockApp_
#define _ClockApp_

#include "BritepadApp.h"
#include "Britepad.h"

class ClockApp : public BritepadApp {
  public:
    ClockApp(void) : BritepadApp() {};
    BritepadApp* run(void);
    bool isScreensaver(void) { return true; };
    const char* name(void) { return "Clock"; };
  private:
    int currentColor = screen.red;

};

#endif

