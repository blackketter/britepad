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

  bool redraw = (last_time != t) || (delta == 0 && (millis()-last_draw > 500));

  if (redraw) {

    char textTime[6];

    sprintf(textTime, t % 2 ? "%d:%02d" : "%d %02d", delta/60, delta%60);

    screen.setTextSize(10);
    coord_t width = screen.measureTextH(textTime);

    last_draw = millis();

    DEBUG_PARAM_LN("delta", delta);

    color_t fg = current_color++;
    color_t bg = bgColor();

    if (delta == 0) {
      delta = timer_dur;

      if (inverse) {
        swap(fg,bg);
      }

// TODO: put the beeper audio into a background app
      if (beeps) {
        beeps--;
        sound.beep();
      }

      inverse = !inverse;
      screen.fillScreen(bg);
    } else if (last_width != width) {
      screen.fillScreen(bg);
    }

    screen.setTextColor(fg, bg);

    screen.setCursor(screen.width()/2 - width/2,
                     screen.height()/2 - screen.measureTextV(textTime)/2);

    screen.drawText(textTime);

    last_time = t;
    last_width = width;
  }

  time_t past = now() - timer_time;

  // exit if any pad is touched
  if (pad.touched(ANY_PAD)) {
    return BACK_APP;
  } else {
    return past > alarm_dur ? DEFAULT_APP : STAY_IN_APP;
  }


}

