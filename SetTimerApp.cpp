#include "SetTimerApp.h"

TimerApp SetTimerApp::timer_app;

SetTimerApp::SetTimerApp(const char* name, time_t duration) : BritepadApp() {
  timer_duration = duration;
  edit = false;
  name_str = name;
}

SetTimerApp::SetTimerApp() : BritepadApp() {

}
bool SetTimerApp::customTimerRunning() {
  return (edit && timer_app.isRunning() && timer_app.getTime() == timer_duration);
}

void SetTimerApp::run() {

  if (customTimerRunning()) {
    setNextApp(&timer_app);
  } else if (!edit) {
    if (timer_app.getTime() != timer_duration) {
      timer_app.setTime(timer_duration);
    }
    setNextApp(&timer_app);
  } else {
    stime_t adj = 0;
    if (button[0].down()) { adj = 60; }
    if (button[1].down()) { adj = -60; }

    if (button[2].down()) { adj = 1; }
    if (button[3].down()) { adj = -1; }

    if (button[4].down()) {
      timer_app.setTime(timer_duration);
      setNextApp(&timer_app);
    }

    if (adj) {
      timer_duration += adj;

      if (timer_duration < 0) { timer_duration = 0; }
      if (timer_duration > 99*60) { timer_duration = 99*60; }

      drawTime();
    }
  }
};


void SetTimerApp::drawTime() {

    char timeStr[100];

    screen.setTextColor(screen.white, bgColor());
    screen.setFont(Arial_72_Bold);

    sprintf(timeStr, " %d:%02d ", timer_duration/60, timer_duration%60);

    screen.setCursor(screen.clipMidWidth() - screen.measureTextWidth(timeStr)/2,
                     screen.clipMidHeight() - screen.measureTextHeight(timeStr)/2);

    screen.drawText(timeStr);
}

void SetTimerApp::drawButtons() {
  for (int i = 0; i < buttoncount; i++) {
    button[i].draw();
  }
}

void SetTimerApp::begin() {
  BritepadApp::begin();

  if (edit && !customTimerRunning()) {
    clearScreen();

    int ytop = screen.clipTop() + screen.clipHeight()/6;
    int ybottom = screen.clipBottom() - screen.clipHeight()/6;

    // todo: fix hard coded pixels because the font is hard coded
    int x1 =  screen.clipMidWidth()/2;
    int x2 =  screen.clipMidWidth();
    int x3 =  x2 + x1;

    coord_t buttonradius = 24;
    button[0].init(x1, ytop, buttonradius,screen.red, false, "+", Arial_32_Bold, screen.white);
    button[1].init(x1, ybottom, buttonradius,screen.red, false, "-", Arial_32_Bold, screen.white);
    button[2].init(x3, ytop, buttonradius,screen.red, false, "+", Arial_32_Bold, screen.white);
    button[3].init(x3, ybottom, buttonradius,screen.red, false, "-", Arial_32_Bold, screen.white);

    button[4].init(x2, ybottom, buttonradius,screen.darkgreen, false, "Start", Arial_12_Bold, screen.black);

    drawTime();
    drawButtons();
  }
}

