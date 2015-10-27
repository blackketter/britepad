#include "AlarmApp.h"
#include "Debug.h"
#include "Preferences.h"


bool AlarmApp::alarmSounding() {
  return (alarmEnabled && ((clock.now()%clock.secsPerDay)/60 == alarmTime.get()/60));
};

struct alarmSettings {
  time_t time;
  bool enabled;
};

void AlarmApp::saveSettings() {
  alarmSettings settings;
  settings.enabled = alarmEnabled;
  settings.time = alarmTime.get();
  prefs.write(id(), sizeof(alarmSettings), (uint8_t*)&settings);
}

void AlarmApp::setAlarmTime(time_t newTime) {
  DEBUG_PARAM_LN("setalarmtime", newTime);
  alarmTime.set(newTime);
  saveSettings();
}

void AlarmApp::setAlarmEnabled(bool alarmOn) {
  alarmEnabled = alarmOn;
  saveSettings();
}


AlarmApp::AlarmApp() {
      alarmSettings settings;
      if (prefs.read(id(), sizeof(alarmSettings), (uint8_t*)&settings)) {
        alarmTime.set(settings.time);
        alarmEnabled = settings.enabled;
      }
};


void AlarmApp::run(void) {
  if (clock.millis() - lastUpdate > beepInterval) {
    char textTime[6];

    screen.setFont(Arial_72_Bold);

    screen.setTextColor(currentColor, bgColor());
    sprintf(textTime, "%d:%02d", alarmTime.hourFormat12(), alarmTime.minute());
    coord_t w = screen.measureTextWidth(textTime);
    coord_t h = screen.measureTextHeight(textTime);
    screen.setCursor(screen.clipMidWidth() - w/2,
                     screen.clipMidHeight() - h/2);
    screen.drawText(textTime);

    if (currentColor == screen.black) {
      currentColor = screen.red;
    } else {
      currentColor = screen.black;
    }

    sound.beep();
    lastUpdate = clock.millis();
  }
};

