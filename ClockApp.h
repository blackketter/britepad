#ifndef _ClockApp_
#define _ClockApp_

#include "ScreensaverApp.h"
#include "Britepad.h"

class ClockApp : public ScreensaverApp {
  public:
    BritepadApp* run(void);
    const char* name(void) { return "Clock"; };
  private:
    time_t lastTime;
    color_t currentColor = screen.red;
};

#endif

