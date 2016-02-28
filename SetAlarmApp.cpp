#include "SetAlarmApp.h"
#include "Debug.h"

void SetAlarmApp::modeButtonPressed() {

 alarmEnabled = !alarmEnabled;

 if (alarmEnabled) {
  sound.beep();
  }
};

void SetAlarmApp::begin() {

  AlarmApp* alarm = (AlarmApp*)getAppByID(AlarmApp::ID);
  if (alarm) {
    alarmTime.set(alarm->getAlarmTime());
    alarmEnabled = alarm->getAlarmEnabled();
  }

  SetClockApp::begin();
}


void SetAlarmApp::end() {

  AlarmApp* alarm = (AlarmApp*)getAppByID(AlarmApp::ID);
  if (alarm) {
    alarm->setAlarmTime(alarmTime.nextOccurance(clock.now()));
    alarm->setAlarmEnabled(alarmEnabled);
  }

  SetClockApp::end();
}
