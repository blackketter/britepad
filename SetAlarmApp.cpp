#include "SetAlarmApp.h"
#include "Debug.h"

void SetAlarmApp::modeButtonPressed() {

 alarmEnabled = !alarmEnabled;

 if (alarmEnabled) {
  sound.beep();
  }
};

void SetAlarmApp::begin() {

  AlarmApp* alarm = (AlarmApp*)getApp(AlarmApp::ID);
  if (alarm) {
    alarmTime.set(alarm->getAlarmTime());
    alarmEnabled = alarm->getAlarmEnabled();
  }

  SetClockApp::begin();
}


void SetAlarmApp::end(BritepadApp* nextApp) {

  AlarmApp* alarm = (AlarmApp*)getApp(AlarmApp::ID);
  if (alarm) {
    alarm->setAlarmTime(alarmTime.nextOccurance());
    alarm->setAlarmEnabled(alarmEnabled);
  }

  SetClockApp::end(nextApp);
}
