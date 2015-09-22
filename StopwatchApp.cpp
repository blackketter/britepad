#include "FastLED.h"

#include "BritepadShared.h"
#include "StopwatchApp.h"
#include "Debug.h"
#include "Clock.h"

void StopwatchApp::begin(void) {
  BritepadApp::begin();

  startMillis = -1;
  lastDrawMillis = millis();

  coord_t diameter = width()/10*2;
  resetButton.init(diameter, top()+height()/3*2-diameter/2, diameter, diameter, screen.blue, false,"Reset");
  pauseButton.init(diameter*3, top()+height()/3*2-diameter/2, diameter, diameter,  screen.green, false, "Start");

  resetButton.draw();
  pauseButton.draw();
}

BritepadApp* StopwatchApp::run(void) {
  millis_t nowMillis = clock.millis();

  if (pad.down(BOTTOM_PAD) || pauseButton.down()) {
    // negative startMillis is the time that we were paused
    if (startMillis > 0) {
      // pause
      startMillis = -(nowMillis - startMillis);
      pauseButton.setColor(screen.green);
      pauseButton.setTitle("Resume");
    } else {
      // unpause
      startMillis = nowMillis + startMillis;
      pauseButton.setColor(screen.red);
      pauseButton.setTitle("Pause");
    }
    sound.click();
  }

  if (resetButton.down()) {
    begin();
  }

  if (lastDrawMillis/100 != nowMillis/100) {
    lastDrawMillis = nowMillis;


    long delta;

    if (startMillis > 0) {
      delta = nowMillis - startMillis;
    }  else {
      delta = -startMillis;
    }

    int hours = delta/(60*60*1000)%60;
    int mins = delta/(60*1000)%60;
    int secs = delta/1000%60;
    int tenths = delta/100%10;


    char textTime[8];

    screen.setFont(Arial_48_Bold);
    screen.setTextColor(screen.white, bgColor());
    if (!hours) {
      sprintf(textTime, "%02d:%02d.%01d", mins, secs, tenths);
    } else {
      sprintf(textTime, "%02d:%02d:%02d", hours, mins, secs);
    }

    coord_t w = screen.measureTextH(textTime);

    screen.setCursor(screen.width()/2 - w/2,
                     screen.height()/3 - screen.measureTextV(textTime)/2);
    screen.drawText(textTime);

    if (startMillis > 0 && secs == 0 && tenths == 0) {
        sound.beep();
    }

  }
  return STAY_IN_APP;
}

