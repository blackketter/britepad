#include "LifeApp.h"
#define DOTSWIDE (32)
#define DOTSHIGH (24)
#define MILLIS_PER_FRAME (200)


BritepadApp* LifeApp::run() {
  if (pad.time() - lastRun < MILLIS_PER_FRAME) {
    return STAY_IN_APP;
  }
  lastRun = pad.time();
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
        nextgen[x][y] = (neighbors == 3);
      } else {
        if (neighbors == 2 || neighbors == 3) {
          // survival
          nextgen[x][y] = true;
        } else {
          nextgen[x][y] = false;
        }
      }
    }
  }
  int population = 0;
  int delta = 0;
  for (int x = 0; x < DOTSWIDE; x++) {
    for (int y = 0; y < DOTSHIGH; y++) {
      color_t newcolor = nextgen[x][y] ? screen.white : screen.black;
      if (newcolor != dots.getDot(x,y)) {
        dots.setDot(x,y, newcolor);
        dots.updateDot(x,y);
        delta++;
      }
      if (nextgen[x][y]) {
        population++;
      }
    }
  }

  if (population == 0 || delta == 0) {
    seed();
  }

  return STAY_IN_APP;
}

void LifeApp::seed(void) {
  dots.clear();
  for (int i = 0; i < 400; i++) {
    dots.setDot(random(DOTSWIDE), random(DOTSHIGH), screen.white);
  }
}

void LifeApp::begin(AppMode asMode) {
  DEBUG_LN("begin LifeApp");
  ScreensaverApp::begin(asMode);
  dots.init(DOTSWIDE,DOTSHIGH,BritepadAppScratchPad);
  seed();
};
