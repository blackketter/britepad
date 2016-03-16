#ifndef _ScribbleClockApp_
#define _ScribbleClockApp_

#include "ClockApp.h"

class ScribbleClockApp : public ClockApp {
  public:
    void update();

    const char* name() { return "Scribble"; };
    appid_t id() { return ID; };
    static constexpr appid_t ID = "scrb";

   virtual millis_t updateInterval() { return 1000; }

  protected:
    millis_t lastErase = 0;

    void drawDigit(int digit, rect_t& r);
 };

#endif



