#ifndef _ClockApp_
#define _ClockApp_

#include "BritepadShared.h"
#include "ScreensaverApp.h"
#include "Timer.h"

class ClockApp : public ScreensaverApp {
  public:
    void begin(AppMode asMode);
    void run();
    virtual void update();
    virtual millis_t updateInterval() { return 1000; }
    virtual Clock& getClock() { return clock; }

    bool displaysClock() { return true; };

    AppType getAppType() { return CLOCK_APP; }

    const char* name() { return "Digital"; };

    bool getEnabled(AppMode asMode) { return ScreensaverApp::getEnabled(asMode) && clock.hasBeenSet(); }

    appid_t id() { return ID; };
    static constexpr appid_t ID = "clok";

  protected:
    millis_t lastUpdate = 0;
    color_t currentColor = screen.red;
    coord_t lastWidth = 0;
};

#endif

