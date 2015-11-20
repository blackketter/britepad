#ifndef _AnalogClockApp_
#define _AnalogClockApp_

#include "BritepadShared.h"
#include "ScreensaverApp.h"
#include "ClockApp.h"

class AnalogClockApp : public ClockApp {
  public:
    void begin();
    void update();
    virtual millis_t updateInterval() { return 100; }

    const char* name() { return "Analog"; };

    appid_t id() { return ID; };
    static constexpr appid_t ID = "anal";

  private:

    const color_t sec_bg = screen.darkred;
    const color_t sec_fg = screen.red;
    const color_t min_bg = screen.darkblue;
    const color_t min_fg = screen.blue;
    const color_t hr_bg = screen.darkgreen;
    const color_t hr_fg = screen.green;
    const color_t center_bg = screen.yellow;

    void dotpos(uint16_t theta, coord_t r, coord_t& x, coord_t& y);
    coord_t last_min_x;
    coord_t last_min_y;
    coord_t last_hr_x;
    coord_t last_hr_y;
    coord_t last_sec_x;
    coord_t last_sec_y;

    millis_t millis_offset;
    uint8_t last_sec;

};

#endif


