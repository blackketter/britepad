#include "LifeApp.h"

#define MILLIS_PER_FRAME (100)
#define MILLIS_DELAY (1000)

#define SEEDS (100)

#define MAX_SAME_POPULATION (5000/MILLIS_PER_FRAME)

#define OFFCOLOR (0x0000)

#define MINCOLOR (0x2104)
#define COLORINC (0x2104)
#define MAXCOLOR (0xe71c)

#define RULESETS (3)
//                                             conway,            maze,               mazectric
const bool born[RULESETS][9] =    {{0,0,0,1,0,0,0,0,0},{0,0,0,1,0,0,0,0,0},{0,1,0,0,0,0,0,0}};
const bool survive[RULESETS][9] = {{0,0,1,1,0,0,0,0,0},{0,1,1,1,1,1,0,0,0},{0,1,1,1,1,0,0,0}};
void LifeApp::begin(AppMode asMode) {

  ScreensaverApp::begin(asMode);

  dots.init(DOTSWIDE,DOTSHIGH,(color_t*)dotData);

  if (asMode == SCREENSAVER) {
    seed();
  }
};

void LifeApp::run() {
  BritepadApp::run();

  switch (getAppMode()) {
    case MOUSE:
    case INTERACTIVE:
      if (pad.down(SCREEN_PAD) && generation) {
        wipe();
      }
      if (pad.touched(SCREEN_PAD)) {
        int x, y;
        if (dots.hit(pad.x(), pad.y(), &x, &y)) {
          dots.setDot(x,y, MAXCOLOR);
          dots.updateDot(x,y);
          nextRun = pad.time() + MILLIS_DELAY;
        }
      }
      // fall through
    case SCREENSAVER:
      iterate();
      break;
    default:
      break;
  }
}

void LifeApp::iterate(void) {
  if (pad.time() > nextRun) {
    nextRun = pad.time() + MILLIS_PER_FRAME;

    if (reseed) {
      seed();
      reseed = false;
    }

    bool nextgen[DOTSWIDE][DOTSHIGH];

    for (int x = 0; x < DOTSWIDE; x++) {
      for (int y = 0; y < DOTSHIGH; y++) {
        int neighbors = 0;
        for (int x1 = x - 1; x1 <= x+1; x1++) {
          for (int y1 = y - 1; y1 <= y+1; y1++) {
            if (
              !(x1 == x && y1 == y) &&
              (x1 >= 0) &&
              (y1 >= 0) &&
              (x1 < DOTSWIDE) &&
              (y1 < DOTSHIGH) &&
              dots.getDot(x1,y1) ) {
                neighbors++;
            }
          }
        }

        color_t curr = dots.getDot(x,y);
        if (!curr) {
          // birth
          nextgen[x][y] = born[ruleset][neighbors];
        } else {
          nextgen[x][y] = survive[ruleset][neighbors];
        }
      }
    }
    int population = 0;
    for (int x = 0; x < DOTSWIDE; x++) {
      for (int y = 0; y < DOTSHIGH; y++) {
        color_t newColor;
        color_t oldColor = dots.getDot(x,y);
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
          dots.setDot(x,y, newColor);
          dots.updateDot(x,y);
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

void LifeApp::wipe(void) {
  dots.clear();
  generation = 0;
  ruleset = random(RULESETS);
}

void LifeApp::seed(void) {
  wipe();
  for (int i = 0; i < SEEDS; i++) {
    dots.setDot(random(DOTSWIDE), random(DOTSHIGH), MINCOLOR);
  }
}
