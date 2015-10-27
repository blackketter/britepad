#ifndef _AlarmApp_
#define _AlarmApp_

#include "BritepadShared.h"
#include "BritepadApp.h"
#include "Clock.h"

class AlarmApp : public BritepadApp {
  public:
    AlarmApp();

    void run(void);
    bool wantsToBeScreensaver() { return alarmSounding(); }
    void end(BritepadApp* nextApp) { BritepadApp::end(nextApp); setAlarmEnabled(false); alarmSounded = false;}

    void setAlarmTime(time_t newTime);
    time_t getAlarmTime() { return alarmTime.get(); };
    bool isAlarmTime();

    void setAlarmEnabled(bool alarmOn);
    bool getAlarmEnabled() { return alarmEnabled; }

    bool alarmSounding();

    const char* name(void) { return "Alarm"; };
    appid_t id() { return ID; };
    static constexpr appid_t ID = "alrm";

  protected:
    millis_t lastUpdate = 0;
    color_t currentColor = screen.red;
    DayTime alarmTime;
    bool alarmEnabled = false;
    bool alarmSounded = false;
    void saveSettings();
    const int alarmDur = 60;  // show the time for a while after the alarm goes off
    const int beepInterval = 500;  // in millis
};

#endif


