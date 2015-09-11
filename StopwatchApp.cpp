#include "FastLED.h"

#include "BritepadShared.h"
#include "StopwatchApp.h"
#include "Debug.h"
#include "Clock.h"

BritepadApp* StopwatchApp::run(void) {
  millis_t nowMillis = clock.millis();

  if (lastDrawMillis/100 != nowMillis/100) {
    lastDrawMillis = nowMillis;


    long delta = nowMillis - startMillis;
    int mins = delta/60000%60;
    int secs = delta/1000%60;
    int tenths = delta/100%10;


    char textTime[8];

    screen.setTextSize(7);
    screen.setTextColor(currentColor, bgColor());
    sprintf(textTime, "%02d:%02d.%01d", mins, secs, tenths);
    coord_t w = screen.measureTextH(textTime);

    screen.setCursor(screen.width()/2 - w/2,
                     screen.height()/2 - screen.measureTextV(textTime)/2);
    screen.drawText(textTime);

    if (secs == 0 && tenths == 0) {
        sound.beep();
    }

  }
  return STAY_IN_APP;
}

