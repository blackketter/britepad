#ifndef _ClockApp_
#define _ClockApp_

#include "BritepadShared.h"
#include "ScreensaverApp.h"
#include "Timer.h"

class ClockApp : public ScreensaverApp {
  public:
    BritepadApp* run(void);
    virtual void update(void);
    virtual millis_t updateInterval() { return 1000; }

    const char* name(void) { return "Clock"; };

    bool getEnabled() { return ScreensaverApp::getEnabled() && clock.hasBeenSet(); }

    appid_t id() { return ID; };
    static constexpr appid_t ID = "clok";

  protected:
    millis_t lastUpdate = 0;
    color_t currentColor = screen.red;
    coord_t lastWidth = 0;
};

#endif

