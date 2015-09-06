#include "BritepadShared.h"
#include "SetClockApp.h"
#include "TimerApp.h"
#include "Clock.h"
#include "Debug.h"

SetClockApp::SetClockApp(void) {
  int ytop = screen.height()/5;
  int ybottom = screen.height() - ytop;
  int spacing = screen.width()/5;
  int x1 = screen.width()/2-2*spacing;
  int x2 =  x1+spacing;
  int x3 =  x2+spacing*2;
  int x4 =  x3+spacing;
  buttony[0] = buttony[1] = buttony[2] = ytop;
  buttonsym[0] = buttonsym[1] = buttonsym[2] = '+';

  buttony[3] = buttony[4] = buttony[5] = ybottom;
  buttonsym[3] = buttonsym[4] = buttonsym[5] ='-';

  buttonx[0] = buttonx[3] = x2;
  buttonx[1] = buttonx[4] = x3;
  buttonx[2] = buttonx[5] = x4;

}

void SetClockApp::drawClock(void) {
  time_t t = now();

  if (lastTime != t) {
    char textTime[6];

    screen.setTextSize(10);
    screen.setTextColor(screen.white, bgColor());
    sprintf(textTime,"%2d:%02d", hourFormat12(t), minute(t));
    screen.setCursor(screen.width()/2 - screen.measureTextH(textTime)/2,
                     screen.height()/2 - screen.measureTextV(textTime)/2);
    screen.drawText(textTime);
    lastTime = t;
  }
}

int SetClockApp::hitButton(int x, int y) {
  for (int i = 0; i < buttoncount; i++) {
    if ((abs(buttonx[i] - x) < buttonradius) && (abs(buttony[i] - y) < buttonradius))
      return i;
  }
  return nobutton;
}

void SetClockApp::drawButton(int i, color_t color) {
  int r = buttonradius;

  screen.fillCircle(buttonx[i],buttony[i],r, color);
  screen.fillRect(buttonx[i] - r/2, buttony[i] - r/8, r, r/4, screen.white);

  if (buttonsym[i] == '+') {
    screen.fillRect(buttonx[i] - r/8, buttony[i] - r/2, r/4, r, screen.white);
  }
}

void SetClockApp::drawButtons() {
  for (int i = 0; i < buttoncount; i++) {
    drawButton(i, screen.red);
  }
}

void SetClockApp::begin(void) {

  screen.fillScreen(bgColor());

  drawClock();
  drawButtons();
  clock.beginSetClock();
}

void SetClockApp::end(BritepadApp* nextApp) {
  clock.endSetClock();
}

BritepadApp* SetClockApp::run(void) {

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
    }
    if (b != nobutton) {
        // reset the minute
        clock.adjust(-now()%60);
        sound.click();
    }

    // todo: cancel timers that may be running
  }

  return STAY_IN_APP;
}


