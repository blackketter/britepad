#include "BritepadShared.h"
#include "SetClockApp.h"
#include "TimerApp.h"
#include "Clock.h"
#include "Debug.h"

void SetClockApp::drawClock(void) {
  time_t t = clock.now();

  if (lastTime != t) {
    char textTime[6];

    screen.setFont(Arial_72);
    screen.setTextColor(screen.white, bgColor());
    sprintf(textTime," %2d:%02d ", clock.hourFormat12(), clock.minute());
    screen.setCursor(width()/2 + left() - screen.measureTextH(textTime)/2,
                     height()/2 + top() - screen.measureTextV(textTime)/2);
    screen.drawText(textTime);
    drawButton(6, screen.red);
    lastTime = t;
  }
}

int SetClockApp::hitButton(int x, int y) {
  int buttonx[buttoncount];
  int buttony[buttoncount];

  int ytop = top() + height()/6;
  int ybottom = bottom() - height()/6;
  buttony[0] = buttony[1] = buttony[2] = buttony[6] = ytop;
  buttony[3] = buttony[4] = buttony[5] = ybottom;

  int xspacing = width()/5;
  int x1 =  left() + xspacing/2;
  int x2 =  x1+xspacing;
  int x3 =  x2+xspacing*2;
  int x4 =  x3+xspacing;

  buttonx[0] = buttonx[3] = x2;
  buttonx[1] = buttonx[4] = x3;
  buttonx[2] = buttonx[5] = x4;
  buttonx[6] = x1;  //  am/pm

  for (int i = 0; i < buttoncount; i++) {
    if ((abs(buttonx[i] - x) < buttonradius) && (abs(buttony[i] - y) < buttonradius))
      return i;
  }
  return nobutton;
}

void SetClockApp::drawButton(int i, color_t color) {
  int buttonx[buttoncount];
  int buttony[buttoncount];
  char buttonsym[buttoncount];

  int ytop = top() + height()/6;
  int ybottom = bottom() - height()/6;
  buttony[0] = buttony[1] = buttony[2] = buttony[6] = ytop;
  buttony[3] = buttony[4] = buttony[5] = ybottom;

  int xspacing = width()/5;
  int x1 =  left() + xspacing/2;
  int x2 =  x1+xspacing;
  int x3 =  x2+xspacing*2;
  int x4 =  x3+xspacing;

  buttonx[0] = buttonx[3] = x2;
  buttonx[1] = buttonx[4] = x3;
  buttonx[2] = buttonx[5] = x4;
  buttonx[6] = x1;  //  am/pm


  buttonsym[0] = buttonsym[1] = buttonsym[2] = '+';
  buttonsym[3] = buttonsym[4] = buttonsym[5] ='-';
  buttonsym[6] = 'm';

  int r = buttonradius;

  screen.fillCircle(buttonx[i],buttony[i],r, color);

  if (buttonsym[i] == 'm') {
    screen.setFont(Arial_20);
    screen.setTextColor(screen.white);
    const char* m = clock.isAM() ? "am" : "pm";
    screen.setCursor(buttonx[i]-screen.measureTextH(m)/2,buttony[i]-screen.measureTextV(m)*3/4);
    screen.drawText(m);
  } else {
    screen.fillRect(buttonx[i] - r/2, buttony[i] - r/8, r, r/4, screen.white);

    if (buttonsym[i] == '+') {
      screen.fillRect(buttonx[i] - r/8, buttony[i] - r/2, r/4, r, screen.white);
    }
  }
}

void SetClockApp::drawButtons() {
  for (int i = 0; i < buttoncount; i++) {
    drawButton(i, screen.red);
  }
}

void SetClockApp::begin(void) {
  lastTime = 0;
  clearScreen();

  drawClock();
  drawButtons();
  clock.beginSetClock();
}

void SetClockApp::end(BritepadApp* nextApp) {
  clock.endSetClock();
}

BritepadApp* SetClockApp::run(void) {

  hasRun = true;
  drawClock();

  if (pad.down(SCREEN_PAD)) {
    int x = pad.x();
    int y = pad.y();
    int b = hitButton(x,y);
    switch (b) {
      case (0):
        clock.adjust(60*60);
      break;
      case (1):
        clock.adjust(60*10);
      break;
      case (2):
        clock.adjust(60);
      break;
      case (3):
        clock.adjust(-60*60);
      break;
      case (4):
        clock.adjust(-60*10);
      break;
      case (5):
        clock.adjust(-60);
      break;
      case (6):
        clock.adjust(clock.isAM() ? 12*60*60 : -12*60*60);
      break;
    }
    if (b != nobutton) {
        // reset the minute
        clock.adjust(-clock.now()%60);
        sound.click();
    }
    // todo: cancel alarms that may be running
  }

  return STAY_IN_APP;
}


