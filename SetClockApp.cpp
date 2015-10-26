#include "BritepadShared.h"
#include "SetClockApp.h"
#include "TimerApp.h"
#include "Debug.h"

enum {
  hr1_up,
  min10_up,
  min1_up,
  hr1_down,
  min10_down,
  min1_down,
  am_pm,
  mode_set
};

void SetClockApp::drawTime(void) {
  millis_t t = clock.millis();

  if (lastDraw/1000 != t/1000) {
    screen.setTextColor(screen.white, bgColor());
    if (setDate) {
      char textDate[2+1+2+1+4+1];

      screen.setFont(Arial_40_Bold);
      sprintf(textDate, "%d-%02d-%02d", theTime->year(), theTime->month(), theTime->day());
      screen.setCursor(screen.clipMidWidth() - screen.measureTextWidth(textDate)/2,
                       screen.clipMidHeight() - screen.measureTextHeight(textDate)/2);
      screen.drawText(textDate);
    } else {
      char textTime[6];

      screen.setFont(Arial_72_Bold);
      sprintf(textTime," %2d:%02d ", theTime->hourFormat12(), theTime->minute());
      screen.setCursor(screen.clipMidWidth() - screen.measureTextWidth(textTime)/2,
                       screen.clipMidHeight() - screen.measureTextHeight(textTime)/2);
      screen.drawText(textTime);
      button[am_pm].setTitle(theTime->isAM() ? "am" : "pm");
      button[am_pm].draw();
    }
  }

  lastDraw = t;
}

void SetClockApp::drawButtons() {
  for (int i = 0; i < buttoncount; i++) {
    button[i].draw();
  }
}

void SetClockApp::begin(AppMode asMode) {
  BritepadApp::begin(asMode);
  lastDraw = 0;
  clearScreen();
  setDate = false;

  int ytop = screen.clipTop() + screen.clipHeight()/6;
  int ybottom = screen.clipBottom() - screen.clipHeight()/6;

  // todo: fix hard coded pixels because the font is hard coded
  int x1 =  screen.clipLeft() + 55;
  int x2 =  x1+60;
  int x3 =  x2+90;
  int x4 =  x3+60;

  button[hr1_up].init(x2, ytop, buttonradius,screen.red, false, "+", Arial_32_Bold, screen.white);
  button[min10_up].init(x3, ytop, buttonradius,screen.red, false, "+", Arial_32_Bold, screen.white);
  button[min1_up].init(x4, ytop, buttonradius,screen.red, false, "+", Arial_32_Bold, screen.white);
  button[hr1_down].init(x2, ybottom, buttonradius,screen.red, false, "-", Arial_32_Bold, screen.white);
  button[min10_down].init(x3, ybottom, buttonradius,screen.red, false, "-", Arial_32_Bold, screen.white);
  button[min1_down].init(x4, ybottom, buttonradius,screen.red, false, "-", Arial_32_Bold, screen.white);

  button[mode_set].init(x1, ytop, buttonradius, modeButtonColor(), false, modeButtonText(), modeButtonFont(), screen.white);
  button[am_pm].init(x1, ybottom, buttonradius,screen.red, false, theTime->isAM() ? "am" : "pm", Arial_18_Bold, screen.white);

  drawTime();
  drawButtons();
  theTime->beginSetTime();
}

void SetClockApp::end(BritepadApp* nextApp) {
  theTime->endSetTime();
}

void SetClockApp::modeButtonPressed() {
    setDate = !setDate;
    button[am_pm].setVisible(!setDate);
    clearScreen();
    drawButtons();
}

void SetClockApp::run(void) {

  hasRun = true;
  stime_t adj = 0;
  bool redraw = false;
  if (button[hr1_up].down()) { adj = setDate ? clock.secsPerYear : clock.secsPerHour; }
  if (button[hr1_down].down()) { adj = setDate ? -clock.secsPerYear : -clock.secsPerHour; }

  if (button[min10_up].down()) { adj = setDate ? clock.daysInMonth(clock.month())*clock.secsPerDay : 60*10; }
  if (button[min10_down].down()) { adj = setDate ? -clock.daysInMonth(clock.month() == 1 ? 12 : clock.month()-1) * clock.secsPerDay : -60*10; }

  if (button[min1_up].down()) { adj = setDate ? clock.secsPerDay : 60; }
  if (button[min1_down].down()) { adj = setDate ? -clock.secsPerDay : -60; }

  if (button[am_pm].down()) { adj = setDate ? : (theTime->isAM() ? 12*60*60 : -12*60*60); }
  if (button[mode_set].down()) {
    modeButtonPressed();
    button[mode_set].setTitle(modeButtonText());
    button[mode_set].setColor(modeButtonColor());
    button[mode_set].setTitleFont(modeButtonFont());
    button[mode_set].draw();
    redraw = true;
  }

  if (adj) { redraw = true; }

  if (redraw) {
    adjust(adj);
    lastDraw = 0;  // force a redraw of the text
    // reset the seconds if we've adjusted the time
    if ((time_t)abs(adj) < clock.secsPerDay) {
      resetSecs();
    }
    // todo: cancel alarms that may be running
  }

  drawTime();
}


