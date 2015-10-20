#include "BritepadShared.h"
#include "SetClockApp.h"
#include "TimerApp.h"
#include "Clock.h"
#include "Debug.h"

void SetClockApp::drawClock(void) {
  time_t t = clock.now();

  if (lastTime != t) {
    screen.setTextColor(screen.white, bgColor());
    if (setDate) {
      char textDate[2+1+2+1+4+1];

      screen.setFont(Arial_40_Bold);
      sprintf(textDate, "%d-%02d-%02d", clock.year(), clock.month(), clock.day());
      screen.setCursor(screen.clipMidWidth() - screen.measureTextWidth(textDate)/2,
                       screen.clipMidHeight() - screen.measureTextHeight(textDate)/2);
      screen.drawText(textDate);
    } else {
      char textTime[6];

      screen.setFont(Arial_72_Bold);
      sprintf(textTime," %2d:%02d ", clock.hourFormat12(), clock.minute());
      screen.setCursor(screen.clipMidWidth() - screen.measureTextWidth(textTime)/2,
                       screen.clipMidHeight() - screen.measureTextHeight(textTime)/2);
      screen.drawText(textTime);
      button[6].setTitle(clock.isAM() ? "am" : "pm");
    }
  }

  lastTime = t;
}

void SetClockApp::drawButtons() {
  for (int i = 0; i < buttoncount; i++) {
    button[i].draw();
  }
}

void SetClockApp::begin(AppMode asMode) {
  lastTime = 0;
  clearScreen();
  setDate = false;

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
  button[7].init(x1, ybottom, buttonradius, screen.red, false, "Date", Arial_12_Bold, screen.white);

  drawClock();
  drawButtons();
  clock.beginSetClock();
}

void SetClockApp::end(BritepadApp* nextApp) {
  clock.endSetClock();
}

BritepadApp* SetClockApp::run(void) {

  hasRun = true;
  time_t adj = 0;
  bool redraw = false;
  if (button[0].down()) { adj = setDate ? clock.secsPerYear : clock.secsPerHour; }
  if (button[3].down()) { adj = setDate ? -clock.secsPerYear : -clock.secsPerHour; }

  if (button[1].down()) { adj = setDate ? clock.daysInMonth(clock.month())*clock.secsPerDay : 60*10; }
  if (button[4].down()) { adj = setDate ? -clock.daysInMonth(clock.month() == 1 ? 12 : clock.month()-1) * clock.secsPerDay : -60*10; }

  if (button[2].down()) { adj = setDate ? clock.secsPerDay : 60; }
  if (button[5].down()) { adj = setDate ? -clock.secsPerDay : -60; }

  if (button[6].down()) { adj = setDate ? : (clock.isAM() ? 12*60*60 : -12*60*60); }
  if (button[7].down()) {
    setDate = !setDate;
    button[6].setVisible(!setDate);
    clearScreen();
    drawButtons();
    redraw = true;
  }

  if (adj) { redraw = true; }

  if (redraw) {
    clock.adjust(adj);
    lastTime = 0;  // force a redraw of the text
    // reset the seconds if we've adjusted the time
    if (abs(adj) < clock.secsPerDay) {
      clock.adjust(-clock.now()%60);
    }
    // todo: cancel alarms that may be running
  }

  drawClock();

  return STAY_IN_APP;
}


