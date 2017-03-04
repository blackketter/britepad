#include "SetTimerApp.h"

SetTimerApp theSetTimerApp(8);

////////////////////////////////////////////////////////////////////////////////////
// SetTimerApp
SetTimerApp::SetTimerApp(int32_t pos) : BritepadApp() {
  setLauncherPosition(pos);
};

void SetTimerApp::run() {

  stime_t adj = 0;
  if (button[0].down()) { adj = 60; }
  if (button[1].down()) { adj = -60; }

  if (button[2].down()) { adj = 1; }
  if (button[3].down()) { adj = -1; }

  if (button[4].down()) {
    theTimerApp.setTime(_customTimerDuration);
    launchApp(&theTimerApp);
  }

  if (adj) {
    _customTimerDuration += adj;
    if (_customTimerDuration < 0) { _customTimerDuration = 0; }
    if (_customTimerDuration > 99*60) { _customTimerDuration = 99*60; }
    drawTime();
  }
};


void SetTimerApp::drawTime() {

    char timeStr[100];

    screen.setTextColor(screen.white, bgColor());
    screen.setFont(Arial_72_Digits_Bold);

    sprintf(timeStr, " %d:%02d ", (int)_customTimerDuration/60, (int)_customTimerDuration%60);

    screen.setCursor(screen.clipMidWidth() - screen.measureTextWidth(timeStr)/2,
                     screen.clipMidHeight() - screen.measureTextHeight(timeStr)/2);

    rect_t clip = { screen.getCursorX(), screen.getCursorY(),(coord_t)screen.measureTextWidth(timeStr),(coord_t)(screen.measureTextHeight(timeStr)+2)};
    screen.pushClipRect(&clip);
    screen.drawText(timeStr);
    screen.pushClipRect(&clip);
}

void SetTimerApp::drawButtons() {
  for (int i = 0; i < _buttonCount; i++) {
    button[i].draw();
  }
}

void SetTimerApp::begin(AppMode asMode) {
  BritepadApp::begin(asMode);

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

void SetTimerApp::init() {
  prefs.read(id(), sizeof(_customTimerDuration), (uint8_t*)&_customTimerDuration);
}

void SetTimerApp::end() {
  prefs.write(id(), sizeof(_customTimerDuration), (uint8_t*)&_customTimerDuration);
}

