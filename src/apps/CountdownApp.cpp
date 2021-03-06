#include "CountdownApp.h"

CountdownApp theCountdownApp;

void CountdownApp::setAppMode(AppMode asMode) {
  BritepadApp::setAppMode(asMode);
  clearScreen();
  redraw();
}

void CountdownApp::redraw() {
  coord_t radius = screen.clipHeight()/10;
  coord_t y = isAppMode(SCREENSAVER_MODE) ? screen.clipTop()+screen.clipHeight()+radius/2 :screen.clipTop()+screen.clipHeight()/4*3;

  setButton.init(screen.clipMidWidth(), y, radius, screen.blue, false,"Set");
  setButton.draw();
  lastDrawMillis = 0;
}

void CountdownApp::begin() {
  ScreensaverApp::begin();
  time_t prefTime;
  if (prefs.get(countdownTimePrefStr, sizeof(prefTime), (uint8_t*)&prefTime)) {
    countdownTime.setSeconds(prefTime);
  } else {
  }

  if (countdownTime.getSeconds() == 0) {
    time_t now = clock.getSeconds();

    countdownTime.setSeconds(now);
  }
  redraw();
};

void CountdownApp::setTime(time_t newTime) {
  countdownTime.setSeconds(newTime);
  prefs.set(countdownTimePrefStr, sizeof(newTime), (uint8_t*)&newTime);
}

void CountdownApp::run() {

  if (pad.pressed(BOTTOM_PAD)) {
    sound.click();
    if (isAppMode(SCREENSAVER_MODE)) {
      setAppMode(INTERACTIVE_MODE);
    } else {
      setAppMode(SCREENSAVER_MODE);
    }
  }

  if (isAppMode(INTERACTIVE_MODE)) {
    if (setButton.pressed()) {
      sound.click();
      launcher.launchApp(&_setCountdownApp);
    }
  }

  // draw the time
  millis_t nowMillis = Uptime::millis();

  if (lastDrawMillis/1000 != nowMillis/1000) {

    lastDrawMillis = nowMillis;
    stime_t delta = countdownTime.getSeconds() - clock.getSeconds();
    bool past = delta < 0;
    delta = abs(delta);

    int days = delta/(60*60*24);
    int hours = (delta/(60*60)) % 24;
    int mins = (delta/60) % 60;
    int secs = delta % 60;
    PString textTime;
    textTime.printf("  %d days, %d hours,  \n", days, hours);
    textTime.printf("  %d minutes, & %d seconds  \n", mins, secs);
    textTime.printf("    %s    \n", past ? "ago..." : "until...");

    screen.setFont(&Arial_16_Bold);
    screen.setTextColor(past ? screen.red : screen.white , bgColor());
    screen.setTextAlign((alignment_t)(ALIGN_HCENTER|ALIGN_VCENTER));
    screen.setCursor(screen.clipMidWidth(),screen.clipMidHeight());

    screen.drawText(textTime.c_str());
    screen.setTextAlign();
  }
}
