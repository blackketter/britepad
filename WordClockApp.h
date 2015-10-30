#ifndef _WordClockApp_
#define _WordClockApp_

#include "BritepadShared.h"
#include "ClockApp.h"

class WordClockApp : public ClockApp {
  public:

    virtual millis_t updateInterval() { return 10000; }

    const char* name() { return "Words"; };

    bool getEnabled() { return ScreensaverApp::getEnabled() && clock.hasBeenSet(); }

    appid_t id() { return ID; };
    static constexpr appid_t ID = "wclk";

  protected:
    virtual void update();
 };

#endif


