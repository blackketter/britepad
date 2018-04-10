#include "AlarmApp.h"
#include "Dictionary.h"

AlarmApp theAlarmApp;

bool AlarmApp::alarmSounding() {
  return (alarmSounded || (alarmEnabled && isAlarmTime()));
};

struct alarmSettings {
  time_t time;
  bool enabled;
};

bool AlarmApp::isAlarmTime() {
  return (clock.now()/60 == nextAlarm.getSeconds()/60);
}

void AlarmApp::saveSettings() {
  alarmSettings settings;
  settings.enabled = alarmEnabled;
  settings.time = nextAlarm.getSeconds();
  prefs.set(id(), sizeof(alarmSettings), (uint8_t*)&settings);
}

void AlarmApp::setAlarmTime(time_t newTime) {
  nextAlarm.setSeconds(newTime);
  updateTimer();
  saveSettings();
}

void AlarmApp::setAlarmEnabled(bool alarmOn) {
  alarmEnabled = alarmOn;
  updateTimer();
  saveSettings();
}

void timerCallback(void* data) {
  ((AlarmApp*)data)->beep();
}

void AlarmApp::beep() {
  if (isAlarmTime()) {
    sound.beep();
    beepTimer.setMillis(beepInterval, timerCallback, (void*)this);
  }
}

void AlarmApp::updateTimer() {
  console.debugln("alarm app updatetimer");

  if (alarmEnabled && nextAlarm.getSeconds()) {
    beepTimer.setClockTime(nextAlarm.getSeconds(), timerCallback, (void*)this);
  } else {
    beepTimer.cancel();
  }
}

void AlarmApp::init() {
//    nextAlarm.setZone(&localTimezone);
    alarmSettings settings;
    if (prefs.get(id(), sizeof(alarmSettings), (uint8_t*)&settings)) {
      nextAlarm.setSeconds(settings.time);
      alarmEnabled = settings.enabled;
    }
};


void AlarmApp::run() {
  if (Uptime::millis() - lastUpdate > beepInterval) {
    alarmSounded = true;

    char textTime[6];
    screen.setFont(Arial_72_Digits_Bold);
    screen.setTextColor(currentColor, bgColor());
    sprintf(textTime, "%d:%02d", nextAlarm.hourFormat12(), nextAlarm.minute());
    coord_t w = screen.measureTextWidth(textTime);
    coord_t h = screen.measureTextHeight(textTime);
    screen.setCursor(screen.clipMidWidth() - w/2,
                     screen.clipMidHeight() - h/2);
    screen.drawText(textTime);
    screen.setFont(Arial_16);
    screen.drawText(clock.isAM() ? "am" : "pm");

    if (currentColor == screen.black) {
      currentColor = screen.red;
    } else {
      currentColor = screen.black;
    }


    lastUpdate = Uptime::millis();
  }
};

