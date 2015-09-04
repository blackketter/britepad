#include <Time.h>
#include "FastLED.h"

#include "BritepadShared.h"
#include "TimerApp.h"
#include "Debug.h"

BritepadApp* TimerApp::run(void) {
  time_t t = now();

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

    screen.setTextSize(10);
    coord_t width = screen.measureTextH(textTime);

    color_t fg = current_color++;
    color_t bg = bgColor();

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
      screen.fillScreen(bg);
    }

    if (drawtext) {
      screen.setTextColor(fg, bg);

      screen.setCursor(screen.width()/2 - width/2,
                       screen.height()/2 - screen.measureTextV(textTime)/2);

      screen.drawText(textTime);
      last_width = width;
    }

    last_time_drawn = t;
  }

  // exit if any pad is touched
  if ( pad.touched(ANY_PAD) || !running) {
    return DEFAULT_APP;
  } else {
    return STAY_IN_APP;
  }
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
  alarm_sounded = now();
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
  mytimer.cancel();
}

