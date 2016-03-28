#ifndef _ScribbleClockApp_
#define _ScribbleClockApp_

#include "ClockApp.h"

class ScribbleClockApp : public ClockApp {
  public:
    void update();
    void begin() { lastH10 = lastH1 = lastM10 = lastM1 = lastS10 = lastS1 = 11;  colons = false; ClockApp::begin(); }
    const char* name() { return "Scribble"; };
    appid_t id() { return ID; };
    static constexpr appid_t ID = "scrb";

   virtual millis_t updateInterval() { return 1000; }

  protected:
    millis_t lastErase = 0;
    int lastH10;
    int lastH1;
    int lastM10;
    int lastM1;
    int lastS10;
    int lastS1;
    bool colons;

    void drawDigit(int digit, rect_t& r);
 };

#endif



