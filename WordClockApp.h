#ifndef _WordClockApp_
#define _WordClockApp_

#include "BritepadShared.h"
#include "ClockApp.h"

class WordClockApp : public ClockApp {
  public:
    void run();
    void end();

    const char* name() { return "Words"; };
    appid_t id() { return ID; };
    static constexpr appid_t ID = "wclk";


    virtual millis_t updateInterval() { return 23000; }

  protected:
    virtual void update();
    uint16_t scrollPos = 0;
 };

#endif
