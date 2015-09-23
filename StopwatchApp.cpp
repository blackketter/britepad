#include "FastLED.h"

#include "BritepadShared.h"
#include "StopwatchApp.h"
#include "Debug.h"
#include "Clock.h"

void StopwatchApp::begin(void) {
  BritepadApp::begin();

  lastDrawMillis = clock.millis();

  coord_t radius = width()/10;
  resetButton.init(radius*3, top()+height()/3*2, radius, screen.blue, false,"Reset");

  pauseButton.init(radius*7, top()+height()/3*2, radius,  startMillis < 0 ? screen.green : screen.red, false, startMillis == -1 ? "Start" : (startMillis < -1 ? "Resume" : "Pause"));

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
    if (pad.down(BOTTOM_PAD)) {
      sound.click();
    }
  }

  if (resetButton.down()) {
    startMillis = -1;
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

