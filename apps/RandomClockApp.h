#ifndef _RandomClockApp_
#define _RandomClockApp_

#include "BritepadShared.h"
#include "ScreensaverApp.h"
#include "ClockApp.h"

class RandomClockApp : public ClockApp {
  public:
    void begin();
    void update();
    virtual millis_t updateInterval() { return 1; }

    const char* name() { return "Random"; };

    appid_t id() { return ID; };
    static constexpr appid_t ID = "rand";

  protected:

  private:
};

#endif


