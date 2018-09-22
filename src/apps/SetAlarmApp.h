#ifndef _SetAlarmApp_
#define _SetAlarmApp_

#include "SetClockApp.h"
#include "AlarmApp.h"

class SetAlarmApp : public SetClockApp {
  public:
    void init() { theTime = &alarmTime; };
    void end();
    void begin(AppMode asMode);

    bool displaysClock() { return false; };

    const char* name() { return "Set Alarm"; };
    appid_t id() { return ID; };
    static constexpr appid_t ID = "salm";

  protected:
    virtual bool highlighted() { return isAlarmEnabled(); }
    virtual font_t modeButtonFont() { return &Arial_18_Bold; }
    virtual const char* modeButtonText() { return alarmEnabled ? "on" : "off"; }
    virtual void modeButtonPressed();
    bool isAlarmEnabled();

  private:
    bool alarmEnabled = false;
    DayTime alarmTime;
};

#endif

