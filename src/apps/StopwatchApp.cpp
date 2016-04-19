#include "FastLED.h"
#include "BritepadShared.h"
#include "StopwatchApp.h"
#include "Debug.h"
#include "Clock.h"

StopwatchApp theStopwatchApp;

void StopwatchApp::begin(AppMode asMode) {
  BritepadApp::begin(asMode);
  if (isReset()) {
    resume();
  }
  redrawButtons();
  redrawTime();
}

void StopwatchApp::switchAppMode(AppMode asMode) {
  BritepadApp::switchAppMode(asMode);
  clearScreen();
  redrawButtons();
  redrawTime();
}

void StopwatchApp::redrawButtons() {
  coord_t radius = screen.clipWidth()/10;
  coord_t y = isAppMode(INTERACTIVE_MODE) ? screen.clipTop()+screen.clipHeight()/4*3 : screen.clipTop()+screen.clipHeight()+radius/2 ;

  resetButton.init(radius*3, y, radius, screen.blue, false,"Reset");

  pauseButton.init(radius*7, y, radius,  isPaused() || isReset() ? screen.green : screen.red, false, isReset() ? "Start" : (isPaused() ? "Resume" : "Pause"));

  resetButton.draw();
  pauseButton.draw();
}

void StopwatchApp::run() {

  BritepadApp::run();

  if (pad.down(BOTTOM_PAD)) {
    sound.click();
    if (isAppMode(INTERACTIVE_MODE)) {
      switchAppMode(MOUSE_MODE);
    } else {
      switchAppMode(INTERACTIVE_MODE);
    }
  }

  if (isAppMode(INTERACTIVE_MODE)) {
    if (pauseButton.down()) {
      sound.click();

      if (!isPaused() && !isReset()) {
        // pause
        pause();
        pauseButton.setColor(screen.green);
        pauseButton.setTitle("Resume");
        pauseButton.draw();
      } else {
        // unpause
        resume();
        pauseButton.setColor(screen.red);
        pauseButton.setTitle("Pause");
        pauseButton.draw();
      }
      redrawTime();
    }

    if (resetButton.down()) {
      sound.click();
      reset();
      redrawButtons();
      redrawTime();
    }
  }

  drawTime();
}

void StopwatchApp::drawTime() {
  millis_t nowMillis = Uptime::millis();

  if (lastDrawMillis/100 != nowMillis/100) {
    lastDrawMillis = nowMillis;
    millis_t delta;

    if (!isPaused()) {
      delta = nowMillis - startMillis;
    }  else {
      delta = -startMillis;
    }

    int hours = (delta / Time::millisPerHour) % 99;
    int mins =  (delta / Time::millisPerMin) % 60;
    int secs =  (delta / Time::millisPerSec) % 60;
    int tenths =(delta / (Time::millisPerSec/10)) % 10;

    char textTime[8];

    screen.setFont(Arial_48_Digits_Bold);
    screen.setTextColor(screen.white, bgColor());
    if (!hours) {
      sprintf(textTime, " %02d:%02d.%01d ", mins, secs, tenths);
    } else {
      sprintf(textTime, "%02d:%02d:%02d", hours, mins, secs);
    }
    static millis_t lastDrawSecs = 0;
    if (lastDrawSecs/1000!=nowMillis/1000) {
      lastDrawSecs = nowMillis;
    }

    coord_t w = screen.measureTextWidth(textTime);

    screen.setCursor(screen.clipMidWidth() - w/2,
                     screen.clipTop() + screen.clipHeight()/(isAppMode(INTERACTIVE_MODE) ? 3 : 2) - screen.measureTextHeight(textTime)/2);
    screen.drawText(textTime);

    if (!isPaused() && secs == 0 && tenths == 0) {
        sound.beep();
    }

  }
}
