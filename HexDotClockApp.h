#ifndef _HexDotClockApp_
#define _HexDotClockApp_

#include "ClockApp.h"
#include "DotMatrix.h"

class HexDotClockApp : public ClockApp {
  public:
    void begin();
    void end();
    void update();


    const char* name() { return "Hex"; };
    appid_t id() { return ID; };
    static constexpr appid_t ID = "hexc";

  protected:
    millis_t lastRedraw = 0;
    color_t currentColor = screen.red;
    coord_t lastWidth = 0;
    static const millis_t redrawInterval = 1000;
    HexDotMatrix* dots = nullptr;

    void drawDigit(uint8_t digit, uint8_t xoff, uint8_t yoff);
 };

#endif


