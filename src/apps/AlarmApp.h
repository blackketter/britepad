#ifndef _AlarmApp_
#define _AlarmApp_

#include "BritepadShared.h"
#include "BritepadApp.h"
#include "Clock.h"

class AlarmApp : public BritepadApp {
  public:
    AlarmApp();

    void run();
    bool wantsToBe(AppMode m) { return alarmSounding(); }
    void end() { BritepadApp::end(); setAlarmEnabled(false); alarmSounded = false;}

    void setAlarmTime(time_t newTime);
    time_t getAlarmTime() { return nextAlarm.getSeconds(); };
    bool isAlarmTime();

    void setAlarmEnabled(bool alarmOn);
    bool getAlarmEnabled() { return alarmEnabled; }

    bool alarmSounding();
    void beep();

    const char* name() { return "Alarm"; };
    appid_t id() { return ID; };
    static constexpr appid_t ID = "alrm";

  protected:

    void updateTimer();

    millis_t lastUpdate = 0;
    color_t currentColor = screen.red;
    Time nextAlarm;
    Timer beepTimer;
    bool alarmEnabled = false;
    bool alarmSounded = false;
    void saveSettings();
    const time_t alarmDur = 60;  // show the time for a while after the alarm goes off
    const millis_t beepInterval = 500;  // in millis
};

#endif


