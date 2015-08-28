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

  if (last_time != t) {
    char textTime[6];
    DEBUG_PARAM_LN("delta", delta);
    screen.setTextSize(10);
    screen.setTextColor(current_color++, screen.black);

    sprintf(textTime, t % 2 ? "%d:%02d" : "%d %02d", delta/60, delta%60);

    screen.setCursor(screen.width()/2 - screen.measureTextH(textTime)/2,
                     screen.height()/2 - screen.measureTextV(textTime)/2);

    screen.drawText(textTime);

    last_time = t;
  }

  time_t past = now() - timer_time;

  return past > display_zeros ? DEFAULT_APP : STAY_IN_APP;

}

