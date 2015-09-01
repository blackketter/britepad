#include <Time.h>
#include "FastLED.h"

#include "Britepad.h"
#include "TimerApp.h"
#include "Debug.h"

BritepadApp* TimerApp::run(void) {
  time_t t = now();

  int delta = timer_time - t;

  if (delta <= 0) {
    delta = 0;
  }

  bool everysecond = (last_time != t);
  bool alarmtriggered = delta == 0;
  bool draw = everysecond;

  if (draw) {
    char textTime[6];
    long displaytime = alarmtriggered ? timer_dur : delta;
    bool drawtext = true;
    bool erasescreen = false;
    const char* flashingcolonformatstring = t % 2 ? "%d:%02d" : "%d %02d";

    sprintf(textTime, flashingcolonformatstring, displaytime/60, displaytime%60);

    screen.setTextSize(10);
    coord_t width = screen.measureTextH(textTime);

    color_t fg = current_color++;
    color_t bg = bgColor();

    if (alarmtriggered) {
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

    last_time = t;

// TODO: put the beeper audio into a background app
    if (alarmtriggered && beeps) {
      beeps--;
      sound.beep();
    }
  }

  time_t past = now() - timer_time;

  // exit if any pad is touched
  if (pad.touched(ANY_PAD)) {
    return DEFAULT_APP;
  } else {
    return past > alarm_dur ? DEFAULT_APP : STAY_IN_APP;
  }


}

