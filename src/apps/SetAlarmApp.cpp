#include "SetAlarmApp.h"
#include "Debug.h"

SetAlarmApp theSetAlarmApp;

void SetAlarmApp::modeButtonPressed() {

 alarmEnabled = !alarmEnabled;

 if (alarmEnabled) {
  sound.beep();
  }
};

void SetAlarmApp::begin(AppMode asMode) {

  AlarmApp* alarm = (AlarmApp*)getAppByID(AlarmApp::ID);
  if (alarm) {
    DEBUGF("getting alarm time of %d\n", alarm->getAlarmTime());
    alarmTime.setSeconds(alarm->getAlarmTime());
    alarmEnabled = alarm->getAlarmEnabled();
  }

  SetClockApp::begin(asMode);
}


void SetAlarmApp::end() {

  AlarmApp* alarm = (AlarmApp*)getAppByID(AlarmApp::ID);
  if (alarm) {
    alarm->setAlarmTime(alarmTime.nextOccurance(clock.now()));
    alarm->setAlarmEnabled(alarmEnabled);
  }

  SetClockApp::end();
}
