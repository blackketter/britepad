#include "SetAlarmApp.h"
#include "Debug.h"

void SetAlarmApp::modeButtonPressed() {

 DEBUG_PARAM_LN("alarmenabled before", alarmEnabled);
 alarmEnabled = !alarmEnabled;
 DEBUG_PARAM_LN("alarmenabled after", alarmEnabled);

 if (alarmEnabled) {
  sound.beep();
  }
};

void SetAlarmApp::begin(AppMode asMode) {

  AlarmApp* alarm = (AlarmApp*)britepad.getApp(AlarmApp::ID);
  if (alarm) {
    alarmTime.set(alarm->getAlarmTime());
    alarmEnabled = alarm->getAlarmEnabled();
    DEBUG_PARAM_LN("starting set alarm app with alarm time", alarmTime.get());
    DEBUG_PARAM_LN("starting set alarm app with alarm enabled", alarmEnabled);
  } else {
    DEBUG_LN("No alarm app!");
  }

  SetClockApp::begin(asMode);
}


void SetAlarmApp::end(BritepadApp* nextApp) {

  AlarmApp* alarm = (AlarmApp*)britepad.getApp(AlarmApp::ID);
  if (alarm) {
    alarm->setAlarmTime(alarmTime.get());
    alarm->setAlarmEnabled(alarmEnabled);
    DEBUG_PARAM_LN("exiting set alarm app with alarm time", alarm->getAlarmTime());
    DEBUG_PARAM_LN("exiting set alarm app with alarmenabled", alarmEnabled);
    DEBUG_PARAM_LN("exiting set alarm app with alarmenabled", alarm->getAlarmEnabled());
    DEBUG_PARAM_LN("that alarm app", (unsigned long)alarm);
  }

  SetClockApp::end(nextApp);
}
