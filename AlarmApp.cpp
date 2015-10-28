#include "AlarmApp.h"
#include "Debug.h"
#include "Preferences.h"


bool AlarmApp::alarmSounding() {
  return (alarmSounded || (alarmEnabled && isAlarmTime()));
};

struct alarmSettings {
  time_t time;
  bool enabled;
};

bool AlarmApp::isAlarmTime(void) {
  return (clock.now()/60 == nextAlarm.get()/60);
}

void AlarmApp::saveSettings() {
  alarmSettings settings;
  settings.enabled = alarmEnabled;
  settings.time = nextAlarm.get();
  prefs.write(id(), sizeof(alarmSettings), (uint8_t*)&settings);
}

void AlarmApp::setAlarmTime(time_t newTime) {
  nextAlarm.set(newTime);
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

void AlarmApp::beep(void) {
  if (isAlarmTime()) {
    sound.beep();
    beepTimer.setMillis(beepInterval, (timerCallback_t)timerCallback, (void*)this);
  }
}

void AlarmApp::updateTimer() {
  if (alarmEnabled && nextAlarm.get()) {
    beepTimer.setClockTime(nextAlarm.get(), (timerCallback_t)timerCallback, (void*)this);
  } else {
    beepTimer.cancel();
  }
}

AlarmApp::AlarmApp() {
      alarmSettings settings;
      if (prefs.read(id(), sizeof(alarmSettings), (uint8_t*)&settings)) {
        nextAlarm.set(settings.time);
        alarmEnabled = settings.enabled;
      }
};


void AlarmApp::run(void) {
  if (clock.millis() - lastUpdate > beepInterval) {
    alarmSounded = true;

    char textTime[6];
    screen.setFont(Arial_72_Bold);
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


    lastUpdate = clock.millis();
  }
};

