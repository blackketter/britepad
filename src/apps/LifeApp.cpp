#include "LifeApp.h"

#define MILLIS_PER_FRAME (100)
#define MILLIS_DELAY (1000)

#define MAX_SAME_POPULATION (5000/MILLIS_PER_FRAME)

#define OFFCOLOR (0x0000)

#define MINCOLOR (0x2104)
#define COLORINC (0x2104)
#define MAXCOLOR (0xe71c)

#define RULESETS (3)
//                                             conway,            maze,               mazectric
const bool born[RULESETS][9] =    {{0,0,0,1,0,0,0,0,0},{0,0,0,1,0,0,0,0,0},{0,1,0,0,0,0,0,0}};
const bool survive[RULESETS][9] = {{0,0,1,1,0,0,0,0,0},{0,1,1,1,1,1,0,0,0},{0,1,1,1,1,0,0,0}};

LifeApp theLifeApp;

void LifeApp::setAppMode(AppMode asMode) {
  DotsDisplayApp::setAppMode(asMode);

  if (asMode == SCREENSAVER_MODE) {
    reseed = true;
  }
}

void LifeApp::begin() {
  _wide = screen.width()/10;
  _high = screen.height()/10;
  DotsDisplayApp::begin();
}

void LifeApp::run() {
  BritepadApp::run();
  bool mouseMoved = ((Uptime::millis() - usbMouse.lastMove()) < 500);
  AppMode mode = getAppMode();

  if (mode == MOUSE_MODE || mode == INTERACTIVE_MODE || mouseMoved) {
    if ((pad.pressed(SCREEN_PAD) || mouseMoved) && generation) {
      wipe();
    }
    coord_t lastX, lastY;
    bool draw = false;
    if (mouseMoved) {
      lastX = usbMouse.x();
      lastY = usbMouse.y();
      draw = true;
    } else if (pad.touched(SCREEN_PAD)) {
      lastX = pad.x();
      lastY = pad.y();
      draw = true;
    }

    if (draw) {
      int x, y;
      if (dots->hit(lastX, lastY, &x, &y)) {
        dots->setDot(x,y, MAXCOLOR);
        dots->updateDot(x,y);
      }
      _nextIterate = Uptime::millis() + MILLIS_DELAY;
    }
  }
  iterate();
}

void LifeApp::iterate() {

  if (Uptime::millis() > _nextIterate) {
    _nextIterate = Uptime::millis() + MILLIS_PER_FRAME;

    if (reseed) {
      seed();
      reseed = false;
    }

    bool nextgen[getDotsWide()][getDotsHigh()];

    for (int x = 0; x < getDotsWide(); x++) {
      for (int y = 0; y < getDotsHigh(); y++) {
        int neighbors = 0;
        for (int x1 = x - 1; x1 <= x+1; x1++) {
          for (int y1 = y - 1; y1 <= y+1; y1++) {
            if (
              !(x1 == x && y1 == y) &&
              (x1 >= 0) &&
              (y1 >= 0) &&
              (x1 < getDotsWide()) &&
              (y1 < getDotsHigh()) &&
              dots->getDot(x1,y1) ) {
                neighbors++;
            }
          }
        }

        color_t curr = dots->getDot(x,y);
        if (!curr) {
          // birth
          nextgen[x][y] = born[ruleset][neighbors];
        } else {
          nextgen[x][y] = survive[ruleset][neighbors];
        }
      }
    }
    int population = 0;
    for (int x = 0; x < getDotsWide(); x++) {
      for (int y = 0; y < getDotsHigh(); y++) {
        color_t newColor;
        color_t oldColor = dots->getDot(x,y);
        if (nextgen[x][y]) {
          if (oldColor < MAXCOLOR) {
            newColor = oldColor + COLORINC;
          } else {
            newColor = oldColor;
          }
        } else {
          newColor = OFFCOLOR;
        }

        if (newColor != oldColor) {
          dots->setDot(x,y, newColor);
          dots->updateDot(x,y);
        }
        if (nextgen[x][y]) {
          population++;
        }
      }
    }

    if (samelastpopulation > MAX_SAME_POPULATION) {
      reseed = true;
      samelastpopulation = 0;
    } else {
      reseed = false;
      generation++;
    }
    if (lastpopulation == population) {
      samelastpopulation++;
    } else {
      samelastpopulation = 0;
    }
    lastpopulation = population;
  }
}

void LifeApp::wipe() {
  dots->clear();
  generation = 0;
  ruleset = random(RULESETS);
}

void LifeApp::seed() {
  console.debugln("LifeApp seeding");
  wipe();
  int seedCount = getDotsWide() * getDotsHigh() / 10;
  for (int i = 0; i < seedCount; i++) {
    dots->setDot(random(getDotsWide()), random(getDotsHigh()), MINCOLOR);
  }
}
