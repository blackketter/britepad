#include "SetAlarmApp.h"
#include "Debug.h"

void SetAlarmApp::modeButtonPressed() {

 alarmEnabled = !alarmEnabled;

 if (alarmEnabled) {
  sound.beep();
  }
};

void SetAlarmApp::begin(AppMode asMode) {

  AlarmApp* alarm = (AlarmApp*)britepad.getApp(AlarmApp::ID);
  if (alarm) {
    alarmTime.set(alarm->getAlarmTime());
    alarmEnabled = alarm->getAlarmEnabled();
  }

  SetClockApp::begin(asMode);
}


void SetAlarmApp::end(BritepadApp* nextApp) {

  AlarmApp* alarm = (AlarmApp*)britepad.getApp(AlarmApp::ID);
  if (alarm) {
    alarm->setAlarmTime(alarmTime.get());
    alarm->setAlarmEnabled(alarmEnabled);
  }

  SetClockApp::end(nextApp);
}
