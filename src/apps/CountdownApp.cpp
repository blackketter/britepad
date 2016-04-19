#include "CountdownApp.h"

CountdownApp theCountdownApp;

void CountdownApp::switchAppMode(AppMode asMode) {
  BritepadApp::switchAppMode(asMode);
  clearScreen();
  redraw();
}

void CountdownApp::redraw() {
  coord_t radius = screen.clipWidth()/10;
  coord_t y = isAppMode(SCREENSAVER_MODE) ? screen.clipTop()+screen.clipHeight()+radius/2 :screen.clipTop()+screen.clipHeight()/4*3;

  setButton.init(screen.clipMidWidth(), y, radius, screen.blue, false,"Set");
  setButton.draw();
  lastDrawMillis = 0;
}

void CountdownApp::begin(AppMode asMode) {
  ScreensaverApp::begin(asMode);
  time_t prefTime;
  if (prefs.read(countdownTimePrefStr, sizeof(prefTime), (uint8_t*)&prefTime)) {
    countdownTime.setSeconds(prefTime);
  } else {
  }

  if (countdownTime.getSeconds() == 0) {
    time_t now = clock.getSeconds();

    countdownTime.setSeconds(now);

  }
};

void CountdownApp::setTime(time_t newTime) {
  countdownTime.setSeconds(newTime);
  prefs.write(countdownTimePrefStr, sizeof(newTime), (uint8_t*)&newTime);
}

void CountdownApp::run() {

  if (pad.down(BOTTOM_PAD)) {
    sound.click();
    if (isAppMode(SCREENSAVER_MODE)) {
      switchAppMode(INTERACTIVE_MODE);
    } else {
      switchAppMode(SCREENSAVER_MODE);
    }
  }

  if (isAppMode(INTERACTIVE_MODE)) {
    if (setButton.down()) {
      sound.click();
      britepad.launchApp(&setApp);
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
    char textTime[100];

    screen.setFont(Arial_16_Bold);
    screen.setTextColor(past ? screen.red : screen.white , bgColor());

    sprintf(textTime, "  %d days, %d hours,  ", days, hours);

    coord_t w = screen.measureTextWidth(textTime);
    coord_t y = screen.clipTop() + screen.clipHeight()/(isAppMode(SCREENSAVER_MODE) ? 2 : 3) - 3*screen.fontLineSpace()/2;

    screen.setCursor(screen.clipMidWidth() - w/2, y);
    screen.drawText(textTime);

    sprintf(textTime, "  %d minutes, & %d seconds  ", mins, secs);
    w = screen.measureTextWidth(textTime);
    y += screen.fontLineSpace();
    screen.setCursor(screen.clipMidWidth() - w/2, y);
    screen.drawText(textTime);

    sprintf(textTime, "    %s    ", past ? "ago..." : "until...");
    w = screen.measureTextWidth(textTime);
    y += screen.fontLineSpace();
    screen.setCursor(screen.clipMidWidth() - w/2, y);
    screen.drawText(textTime);

  }
}
