#ifndef _CalendarApp_
#define _CalendarApp_
#include "BritepadApp.h"
#include "widgets/Button.h"

class CalendarApp : public BritepadApp {
  public:
    void begin(AppMode asMode) { BritepadApp::begin(asMode);  year = clock.year(); month = clock.month(); draw(); }
    void run();

    bool disablesScreensavers() { return true; }

    const char* name() { return "Calendar"; };
    appid_t id() { return ID; };
    static constexpr appid_t ID = "cala";
    AppType getAppType() { return INTERACTIVE_APP; }

  private:

    uint16_t year;
    uint8_t month;
    void draw();
};

#endif


