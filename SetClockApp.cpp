#include "BritepadShared.h"
#include "SetClockApp.h"
#include "TimerApp.h"
#include "Clock.h"
#include "Debug.h"

void SetClockApp::drawClock(void) {
  time_t t = clock.now();

  if (lastTime != t) {
    char textTime[6];

    screen.setFont(Arial_72_Bold);
    screen.setTextColor(screen.white, bgColor());
    sprintf(textTime," %2d:%02d ", clock.hourFormat12(), clock.minute());
    screen.setCursor(screen.clipMidWidth() - screen.measureTextH(textTime)/2,
                     screen.clipMidHeight() - screen.measureTextV(textTime)/2);
    screen.drawText(textTime);
    button[6].setTitle(clock.isAM() ? "am" : "pm");
    lastTime = t;
  }
}

void SetClockApp::drawButtons() {
  for (int i = 0; i < buttoncount; i++) {
    button[i].draw();
  }
}

void SetClockApp::begin(AppMode asMode) {
  lastTime = 0;
  clearScreen();

  int ytop = screen.clipTop() + screen.clipHeight()/6;
  int ybottom = screen.clipBottom() - screen.clipHeight()/6;

  // todo: fix hard coded pixels because the font is hard coded
  int x1 =  screen.clipLeft() + 55;
  int x2 =  x1+60;
  int x3 =  x2+90;
  int x4 =  x3+60;

  button[0].init(x2, ytop, buttonradius,screen.red, false, "+", Arial_32_Bold, screen.white);
  button[1].init(x3, ytop, buttonradius,screen.red, false, "+", Arial_32_Bold, screen.white);
  button[2].init(x4, ytop, buttonradius,screen.red, false, "+", Arial_32_Bold, screen.white);
  button[3].init(x2, ybottom, buttonradius,screen.red, false, "-", Arial_32_Bold, screen.white);
  button[4].init(x3, ybottom, buttonradius,screen.red, false, "-", Arial_32_Bold, screen.white);
  button[5].init(x4, ybottom, buttonradius,screen.red, false, "-", Arial_32_Bold, screen.white);
  button[6].init(x1, ytop, buttonradius,screen.red, false, clock.isAM() ? "am" : "pm", Arial_18_Bold, screen.white);

  drawClock();
  drawButtons();
  clock.beginSetClock();
}

void SetClockApp::end(BritepadApp* nextApp) {
  clock.endSetClock();
}

BritepadApp* SetClockApp::run(void) {

  hasRun = true;
  bool changed = false;
  if (button[0].down()) { clock.adjust(60*60); changed = true; }
  if (button[1].down()) { clock.adjust(60*10); changed = true; }
  if (button[2].down()) { clock.adjust(60); changed = true; }
  if (button[3].down()) { clock.adjust(-60*60); changed = true; }
  if (button[4].down()) { clock.adjust(-60*10); changed = true; }
  if (button[5].down()) { clock.adjust(-60); changed = true; }
  if (button[6].down()) { clock.adjust(clock.isAM() ? 12*60*60 : -12*60*60); changed = true; }

  if (changed) {
    // reset the minute
    clock.adjust(-clock.now()%60);
    sound.click();
    // todo: cancel alarms that may be running
  }

  drawClock();

  return STAY_IN_APP;
}


