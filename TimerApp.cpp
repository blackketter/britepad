#include "FastLED.h"

#include "BritepadShared.h"
#include "TimerApp.h"
#include "Debug.h"
#include "Clock.h"

void TimerApp::end(BritepadApp* nextApp) {
  // don't bother coming back as a screensaver if the alarm has sounded
  if (alarm_sounded) {
    mytimer.cancel();
    beeps = 0;
  }
}

void TimerApp::drawTime(void) {

  int delta = mytimer.remainingSecs();

  if (delta <= 0) {
    delta = 0;
  }

  millis_t now = clock.millis();
  if (lastDrawMillis/1000 != now/1000) {
    lastDrawMillis = now;

    char textTime[6];

    int displaytime = alarm_sounded ? timer_dur : delta;
    color_t textColor = screen.white;

    sprintf(textTime, " %d:%02d ", displaytime/60, displaytime%60);

    screen.setFont(isAppMode(SCREENSAVER) ? Arial_72_Bold : Arial_48_Bold);
    coord_t width = screen.measureTextH(textTime);

    if (alarm_sounded) {
      if ((now/1000) % 2) {
        textColor = screen.red;
      }
    }

    screen.setTextColor(textColor, bgColor());

    screen.setCursor(screen.clipMidWidth() - width/2,
                     screen.clipTop() + screen.clipHeight()/(isAppMode(SCREENSAVER) ? 2 : 3) - screen.measureTextV(textTime)/2);

    screen.drawText(textTime);

    last_width = width;
  }

}

void alarmcallback(void* data) {
  ((TimerApp*)data)->alarm();
}

void TimerApp::setTime(time_t t) {
  beeps = 9;
  timer_dur = t;
  alarm_sounded = 0;
  mytimer.setSecs(t, (timerCallback_t)alarmcallback, (void*)this);
};

void TimerApp::alarm(void) {
  alarm_sounded = clock.now();
  if (beeps) {
    sound.beep();
    mytimer.setMillis(500, (timerCallback_t)alarmcallback, (void*)this);
    beeps--;
  } else {
    mytimer.setSecs(alarm_dur);  // keep the timer running a while to keep the app on screen, when it's done it will exit automatically because the timer isn't running
  }
}

void TimerApp::reset(void) {
  beeps = 0;
  mytimer.cancel();
  setTime(timer_dur);
  pause();
}

