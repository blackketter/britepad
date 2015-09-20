#include "FastLED.h"

#include "BritepadShared.h"
#include "TimerApp.h"
#include "Debug.h"
#include "Clock.h"

void TimerApp::end(BritepadApp* nextApp) {
  // don't bother coming back as a screensaver if the alarm has sounded
  if (alarm_sounded) {
    cancel();
  }
}

BritepadApp* TimerApp::run(void) {
  time_t t = clock.now();

  int delta = mytimer.remainingSecs();

  if (delta <= 0) {
    delta = 0;
  }

  bool everysecond = (last_time_drawn != t);
  bool draw = everysecond;

  if (draw) {
    char textTime[6];

    long displaytime = alarm_sounded ? timer_dur : delta;
    bool drawtext = true;
    bool erasescreen = false;

    const char* flashingcolonformatstring = t % 2 ? "%d:%02d" : "%d %02d";

    sprintf(textTime, flashingcolonformatstring, displaytime/60, displaytime%60);

    screen.setFont(Arial_72);
    coord_t width = screen.measureTextH(textTime);

    color_t fg = current_color++;

    if (alarm_sounded) {
      if (t % 2) {
        erasescreen = false;
        drawtext = true;
      } else {
        erasescreen = true;
        drawtext = false;
      }
    } else {
      if (last_width != width) {
        erasescreen = true;
      }
    }

    if (erasescreen) {
      clearScreen();
    }

    if (drawtext) {
      screen.setTextColor(fg, bgColor());

      screen.setCursor(screen.width()/2 - width/2,
                       screen.height()/2 - screen.measureTextV(textTime)/2);

      screen.drawText(textTime);
      last_width = width;
    }

    last_time_drawn = t;
  }

  if (pad.down(BOTTOM_PAD)) {
    if (mytimer.isPaused()) {
      mytimer.resume();
    } else {
      mytimer.pause();
    }
  }

  return running ? STAY_IN_APP : DEFAULT_APP;
}

void alarmcallback(void* data) {
  ((TimerApp*)data)->alarm();
}

void cancelVisualNotification(void* data) {
  ((TimerApp*)data)->cancel();
}

void TimerApp::setTime(time_t t) {
  beeps = 9;
  timer_dur = t;
  alarm_sounded = 0;
  running = true;
  mytimer.setSecs(t, (timerCallback_t)alarmcallback, (void*)this);
};

void TimerApp::alarm(void) {
  alarm_sounded = clock.now();
  if (beeps) {
    sound.beep();
    mytimer.setMillis(1000, (timerCallback_t)alarmcallback, (void*)this);
    beeps--;
  } else {
    mytimer.setSecs(5*60, (timerCallback_t)cancelVisualNotification, (void*)this);
  }
}

void TimerApp::cancel(void) {
  running = false;
  beeps = 0;
  mytimer.cancel();
}

