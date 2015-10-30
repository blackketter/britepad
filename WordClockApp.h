#ifndef _WordClockApp_
#define _WordClockApp_

#include "BritepadShared.h"
#include "ClockApp.h"

class WordClockApp : public ClockApp {
  public:
    const char* name() { return "Words"; };
    appid_t id() { return ID; };
    static constexpr appid_t ID = "wclk";

    virtual millis_t updateInterval() { return 10000; }

  protected:
    virtual void update();
 };

#endif


