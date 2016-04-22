#include "SetTimerApp.h"

TimerApp SetTimerApp::timer_app;

SetTimerApp timer10sec("10 sec", 10,0);
SetTimerApp timer3min("3 min", 3*60,1);
SetTimerApp timer25min("25 min", 25*60,2);
SetTimerApp timer55min("55 min", 55*60,3);
SetTimerApp customTimerApp("Set Timer", 0, 8);

SetTimerApp::SetTimerApp(const char* name, time_t duration, int32_t pos) : BritepadApp() {
  timer_duration = duration;
  edit = duration == 0;
  name_str = name;
  setLauncherPosition(pos);
}

SetTimerApp::SetTimerApp() : BritepadApp() {

}
bool SetTimerApp::customTimerRunning() {
  return (edit && timer_app.isRunning() && timer_app.getTime() == timer_duration);
}

void SetTimerApp::run() {

  if (customTimerRunning()) {
    launchApp(&timer_app);
  } else if (!edit) {
    if (timer_app.getTime() != timer_duration || timer_app.isReset()) {
      timer_app.setTime(timer_duration);
    }
    launchApp(&timer_app);
  } else {
    stime_t adj = 0;
    if (button[0].down()) { adj = 60; }
    if (button[1].down()) { adj = -60; }

    if (button[2].down()) { adj = 1; }
    if (button[3].down()) { adj = -1; }

    if (button[4].down()) {
      timer_app.setTime(timer_duration);
      launchApp(&timer_app);
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
    screen.setFont(Arial_72_Digits_Bold);

    sprintf(timeStr, " %d:%02d ", timer_duration/60, timer_duration%60);

    screen.setCursor(screen.clipMidWidth() - screen.measureTextWidth(timeStr)/2,
                     screen.clipMidHeight() - screen.measureTextHeight(timeStr)/2);

    rect_t clip = { screen.getCursorX(), screen.getCursorY(),(coord_t)screen.measureTextWidth(timeStr),(coord_t)(screen.measureTextHeight(timeStr)+2)};
    screen.pushClipRect(&clip);
    screen.drawText(timeStr);
    screen.pushClipRect(&clip);
}

void SetTimerApp::drawButtons() {
  for (int i = 0; i < buttoncount; i++) {
    button[i].draw();
  }
}

void SetTimerApp::begin(AppMode asMode) {
  BritepadApp::begin(asMode);

  if (edit && !customTimerRunning()) {
    clearScreen();

    int ytop = screen.clipTop() + screen.clipHeight()/6;
    int ybottom = screen.clipBottom() - screen.clipHeight()/6;

    // todo: fix hard coded pixels because the font is hard coded
    int x1 =  screen.clipMidWidth()/2;
    int x2 =  screen.clipMidWidth();
    int x3 =  x2 + x1;

    coord_t buttonradius = 24;
    button[0].init(x1, ytop, buttonradius,screen.red, false, screen.white, plusIcon);
    button[1].init(x1, ybottom, buttonradius,screen.red, false, screen.white, minusIcon);
    button[2].init(x3, ytop, buttonradius,screen.red, false, screen.white, plusIcon);
    button[3].init(x3, ybottom, buttonradius,screen.red, false,screen.white, minusIcon);

    button[4].init(x2, ybottom, buttonradius,screen.darkgreen, false, "Start", Arial_12_Bold, screen.black);

    drawTime();
    drawButtons();
  }
}

