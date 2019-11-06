#ifndef _HexDotClockApp_
#define _HexDotClockApp_

#include "ClockApp.h"
#include "widgets/DotMatrix.h"

class HexDotClockApp : public ClockApp {
  public:
    void begin(AppMode asMode);
    void end();
    void update();
    millis_t updateInterval() override { return 60000; }

    const char* name() { return "Hex"; };
    appid_t id() { return ID; };
    static constexpr appid_t ID = "hexc";

  protected:
    HexDotMatrix* dots = nullptr;
    void drawDigit(uint8_t digit, uint8_t xoff, uint8_t yoff);
 };

#endif


