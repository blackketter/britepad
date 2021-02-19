#include "SetAlarmApp.h"

SetAlarmApp theSetAlarmApp;

void SetAlarmApp::modeButtonPressed() {

 alarmEnabled = !alarmEnabled;

 if (alarmEnabled) {
  sound.beep();
  }
};

void SetAlarmApp::begin() {
  AlarmApp* alarm = (AlarmApp*)BritepadApp::getAppByID(AlarmApp::ID);
  if (alarm) {
    console.debugf("getting alarm time of %d\n", alarm->getAlarmTime());
    alarmTime.setSeconds(alarm->getAlarmTime());
    alarmEnabled = alarm->getAlarmEnabled();
  }

  SetClockApp::begin();
}

void SetAlarmApp::end() {

  AlarmApp* alarm = (AlarmApp*)BritepadApp::getAppByID(AlarmApp::ID);
  if (alarm) {
    alarm->setAlarmTime(alarmTime.nextOccurance(clock.now()));
    alarm->setAlarmEnabled(alarmEnabled);
  }

  SetClockApp::end();
}

bool SetAlarmApp::isAlarmEnabled() {
  AlarmApp* alarm = (AlarmApp*)BritepadApp::getAppByID(AlarmApp::ID);
  if (alarm) {
    return alarm->getAlarmEnabled();
  } else {
    return false;
  }
}
