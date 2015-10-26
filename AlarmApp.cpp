#include "AlarmApp.h"
#include "Debug.h"



bool AlarmApp::alarmSounding() {
  DEBUG_PARAM_LN("alarmsounding: alarm enabled", alarmEnabled);
  DEBUG_PARAM_LN("alarm sounding now", clock.now()%clock.secsPerDay/60);
  DEBUG_PARAM_LN("alarm sounding alarmtime", alarmTime.get()/60);
  DEBUG_PARAM_LN("this alarmapp", (unsigned long)this);

  return (alarmEnabled && ((clock.now()%clock.secsPerDay)/60 == alarmTime.get()/60));
};

void AlarmApp::setAlarmTime(time_t newTime) {
  DEBUG_PARAM_LN("setalarmtime", newTime);
  alarmTime.set(newTime);

}

// todo save time to prefs
// todo load saved time
AlarmApp::AlarmApp() {

};


void AlarmApp::run(void) {
  if (clock.millis() - lastUpdate > beepInterval) {
    DEBUG_LN("alarmapp::run");
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

