#ifndef _ZoneClockApp_
#define _ZoneClockApp_
#include "ClockApp.h"

class ZoneClockApp : public ClockApp {
  public:
    ZoneClockApp(const char* name, stime_t offset);
    const char* name();
    virtual void update();

    virtual bool isAppType(AppType t) { return t & (KEY_APP | CLOCK_APP); }  // have it appear both places

    appid_t id() { return ID; };
    static constexpr appid_t ID = "zone";

  protected:
    const char* _offsetName;
    char _nameStr[100];
};

#endif
