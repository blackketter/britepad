#ifndef _BinaryClockApp_
#define _BinaryClockApp_

#include "ClockApp.h"

class BinaryClockApp : public ClockApp {
  public:
    void update();

    const char* name() { return "Binary"; };
    appid_t id() { return ID; };
    static constexpr appid_t ID = "binc";

  protected:
    millis_t lastRedraw = 0;
    color_t currentColor = screen.red;
    coord_t lastWidth = 0;
    static const millis_t redrawInterval = 1000;
};

#endif


