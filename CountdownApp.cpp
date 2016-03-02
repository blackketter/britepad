#include "CountdownApp.h"

void CountdownApp::setAppMode(AppMode asMode) {
  DEBUGF("Countdown setappmode\n");
  BritepadApp::setAppMode(asMode);
  clearScreen();
  redraw();
}

void CountdownApp::redraw() {
  DEBUGF("Countdown redraw\n");
  coord_t radius = screen.clipWidth()/10;
  coord_t y = isAppMode(SCREENSAVER_MODE) ? screen.clipTop()+screen.clipHeight()+radius/2 :screen.clipTop()+screen.clipHeight()/4*3;

  setButton.init(screen.clipMidWidth(), y, radius, screen.blue, false,"Set");
  setButton.draw();
  lastDrawMillis = 0;
}

void CountdownApp::begin() {
  DEBUGF("Countdown begin\n");
  if (prefs.read(countdownTimePrefStr, sizeof(countdownTime), (uint8_t*)&countdownTime)) {
    DEBUGF("Countdown time of %d\n", countdownTime.getSeconds());
  } else {
    DEBUG_LN("No countdown pref found");
  }

  if (countdownTime.getSeconds() == 0) {
    time_t now = clock.getSeconds();

    DEBUGF("Countdown now: %d\n", now);
    DEBUG_LN("Countdown Time zero, resetting to now.");
    //countdownTime.setMicros(now);
    countdownTime.setSeconds(now);
    //countdownTime.setMicros(((micros_t)now)*1000000);
    //countdownTime.setMicros( ((micros_t)now) * Time::microsPerSec );
    DEBUG_LN("Countdown Done.");


  }
};

void CountdownApp::setTime(time_t newTime) {
//  DEBUGF("setMicros");
//  countdownTime.setMicros(newTime);
  DEBUGF("setSeconds");
  countdownTime.setSeconds(newTime);
  DEBUGF("Countdown writepref\n", newTime);
  prefs.write(countdownTimePrefStr, sizeof(countdownTime), (uint8_t*)&countdownTime);
}

void CountdownApp::run() {

  if (pad.down(BOTTOM_PAD)) {
    sound.click();
    if (isAppMode(SCREENSAVER_MODE)) {
      setAppMode(INTERACTIVE_MODE);
    } else {
      setAppMode(SCREENSAVER_MODE);
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
    DEBUGF("Countdown drawing\n");
    stime_t delta = countdownTime.getSeconds() - clock.getSeconds();
    DEBUGF("Countdown delta: %d\n", delta);
    bool past = delta < 0;
    delta = abs(delta);

    int days = delta/(60*60*24);
    int hours = (delta/(60*60)) % 24;
    int mins = (delta/60) % 60;
    int secs = delta % 60;
    DEBUGF("countdown: D:%d h:%d m:%d s:%d total:%d\n", days, hours,mins, secs, delta);
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
