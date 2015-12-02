#include "FastLED.h"

#include "BritepadShared.h"
#include "TimerApp.h"
#include "Debug.h"
#include "Clock.h"

void TimerApp::end() {
  // don't bother coming back as a screensaver if the alarm has sounded
  if (alarm_sounded) {
    mytimer.cancel();
    beeps = 0;
    alarm_sounded = 0;
  }
}

void TimerApp::drawTime() {

  int delta = mytimer.remainingSecs();

  if (delta <= 0) {
    delta = 0;
  }

  millis_t now = clock.millis();
  if (lastDrawMillis/redrawInterval != now/redrawInterval) {
    lastDrawMillis = now;

    char textTime[100];

    int displaytime = alarm_sounded ? timer_dur : delta;
    color_t textColor = screen.white;

    sprintf(textTime, " %d:%02d ", displaytime/60, displaytime%60);

    screen.setFont(Arial_72_Bold);
    coord_t width = screen.measureTextWidth(textTime);

    if (alarm_sounded) {
      if ((now/redrawInterval) % 2 || (displaytime == 0)) {
        textColor = screen.red;
      } else {
        textColor = screen.grey10;
      }
    }

    screen.setTextColor(textColor, bgColor());

    screen.setCursor(screen.clipMidWidth() - width/2,
                     screen.clipTop() + screen.clipHeight()/(isAppMode(INTERACTIVE_MODE) ? 3 : 2) - screen.measureTextHeight(textTime)/2);

    rect_t clip = { screen.getCursorX(), screen.getCursorY(),(coord_t)screen.measureTextWidth(textTime),(coord_t)(screen.measureTextHeight(textTime)+2)};
    screen.pushClipRect(&clip);
    screen.drawText(textTime);
    screen.pushClipRect(&clip);

    if (alarm_sounded && !isAppMode(INTERACTIVE_MODE)) {
      long past = clock.now() - alarm_sounded;
      sprintf(textTime, "%2d:%02d", (int)(past / 60), (int)(past % 60));
      screen.setFont(Arial_20_Bold);
      screen.setTextColor(screen.red, bgColor());
      screen.setCursor(screen.clipMidWidth() - screen.measureTextWidth(textTime)/2,
                     screen.clipTop() + screen.clipHeight()/6 - screen.measureTextHeight(textTime)/2);
      screen.drawText(textTime);
    }
    last_width = width;
  }

}

void alarmcallback(void* data) {
  ((TimerApp*)data)->alarm();
}

void TimerApp::setTime(time_t t) {
  beeps = totalBeeps -1;
  timer_dur = t;
  alarm_sounded = 0;
  mytimer.setSecs(t, (timerCallback_t)alarmcallback, (void*)this);
};

void TimerApp::alarm() {
  if (!alarm_sounded) {
    alarm_sounded = clock.now();
  }

  if (beeps) {
    sound.beep();
    mytimer.setMillis(beepInterval, (timerCallback_t)alarmcallback, (void*)this);
    beeps--;
  } else {
    mytimer.setSecs(alarmDur);  // keep the timer running a while to keep the app on screen, when it's done it will exit automatically because the timer isn't running
  }
}

void TimerApp::reset() {
  beeps = 0;
  mytimer.cancel();
  setTime(timer_dur);
  pause();
}

