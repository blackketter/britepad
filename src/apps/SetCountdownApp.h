#ifndef _SetCountdownApp_
#define _SetCountdownApp_

#include "SetClockApp.h"
#include "AlarmApp.h"

class SetCountdownApp : public SetClockApp {
  public:
    SetCountdownApp() { theTime = &countdownTime;};
    void end();
    void begin(AppMode asMode);

    BritepadApp* exitsTo();
    bool displaysClock() { return false; };

    const char* name() { return "Set Countdown"; };
    appid_t id() { return ID; };
    static constexpr appid_t ID = "sctd";
    AppType getAppType() { return NO_APP_TYPE; }

  protected:
    Time countdownTime;

  private:
};

#endif


