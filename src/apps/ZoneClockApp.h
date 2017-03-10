#ifndef _ZoneClockApp_
#define _ZoneClockApp_
#include "ClockApp.h"

class ZoneClockApp : public ClockApp {
  public:
    ZoneClockApp(const char* name, TimeChangeRule dstStart, TimeChangeRule stdStart);
    const char* name();
    virtual void update();

    virtual bool isAppType(AppType t) { return t & (KEY_APP | CLOCK_APP); }  // have it appear both places

    appid_t id() { return ID; };
    static constexpr appid_t ID = "zone";

    virtual Clock& getClock() { return _zoneClock; }

  protected:
    const char* _offsetName;
    char _nameStr[100];
    Clock _zoneClock;
    Timezone _zone;
};

#endif
