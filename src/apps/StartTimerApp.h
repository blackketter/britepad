#ifndef _StartTimerApp_
#define _StartTimerApp_

#include "BritepadApp.h"
#include "BritepadShared.h"
#include "TimerApp.h"
#include "SetTimerApp.h"

class StartTimerApp : public BritepadApp {
  public:
    StartTimerApp(const char* name, time_t duration, int32_t pos = defaultLauncherPosition);
    StartTimerApp();
    void run();

    color_t appButtonColor() { return screen.LightBlue; }

    virtual bool highlighted() { return !theTimerApp.isReset() && (theTimerApp.getTime() == getDuration()); }

    const char* name() { return _nameStr; };
    appid_t id() { return ID; };
    static constexpr appid_t ID = "stmr";
    AppType getAppType() { return TIMER_APP; }
    virtual int32_t getDuration() { return _timerDuration; }

  private:
    const char* _nameStr;
    int32_t _timerDuration = 0;
};

class StartCustomTimerApp : public StartTimerApp {
  public:
    StartCustomTimerApp(int32_t pos);
    const char* name() {
      sprintf(_customTimerName, "%2d:%02d", (int)getDuration() / 60, (int)getDuration() % 60);
      return _customTimerName;
    }

    appid_t id() { return ID; };
    static constexpr appid_t ID = "sctm";

    int32_t getDuration() { return theSetTimerApp.getDuration(); }

  private:
    char _customTimerName[6];
};

#endif
