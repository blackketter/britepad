#ifndef _CalendarApp_
#define _CalendarApp_
#include "BritepadApp.h"
#include "Button.h"
#include "Icons.h"


class CalendarApp : public BritepadApp {
  public:
    void begin() { BritepadApp::begin();  year = clock.year(); month = clock.month(); draw(); }
    void run();

    bool disablesScreensavers() { return true; }

    const char* name() { return "Calendar"; };
    appid_t id() { return ID; };
    static constexpr appid_t ID = "cala";

  private:

    uint16_t year;
    uint8_t month;
    void draw();
};

#endif


