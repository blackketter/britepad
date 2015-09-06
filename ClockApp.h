#ifndef _ClockApp_
#define _ClockApp_

#include "BritepadShared.h"
#include "ScreensaverApp.h"
#include "Timer.h"

class ClockApp : public ScreensaverApp {
  public:
    BritepadApp* run(void);
    virtual void redraw(void);
    const char* name(void) { return "Clock"; };

    void chimerCallback();
  private:
    millis_t lastRedraw;
    color_t currentColor = screen.red;
    coord_t lastWidth = 0;
    static const millis_t redrawInterval = 1000;
};

#endif

