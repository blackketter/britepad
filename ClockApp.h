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

    bool getEnabled() { return ScreensaverApp::getEnabled() && clock.hasBeenSet(); }

    appid_t id() { return ID; };
    static constexpr appid_t ID = "clok";

  protected:
    millis_t lastRedraw = 0;
    color_t currentColor = screen.red;
    coord_t lastWidth = 0;
    static const millis_t redrawInterval = 1000;
};

#endif

