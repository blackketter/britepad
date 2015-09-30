#include "LifeApp.h"
#define DOTSWIDE (32)
#define DOTSHIGH (24)
#define MILLIS_PER_FRAME (150)
#define MILLIS_DELAY (1000)
#define ONCOLOR (screen.white)
#define OFFCOLOR (screen.black)


void LifeApp::begin(AppMode asMode) {
  DEBUG_LN("begin LifeApp");

  ScreensaverApp::begin(asMode);

  if (asMode == MOUSE) {
    mouse.begin();
  }

  dots.init(DOTSWIDE,DOTSHIGH,BritepadAppScratchPad);

  if (asMode == SCREENSAVER) {
    seed();
  }
};

void LifeApp::end(void) {
  if (isAppMode(MOUSE)) {
    mouse.end();
  }
}

void LifeApp::setAppMode( AppMode newMode ) {
  if (!isAppMode(newMode)) {
    if (newMode == MOUSE) {
      mouse.begin();
    } else if (isAppMode(MOUSE)) {
      mouse.end();
    }
    ScreensaverApp::setAppMode(newMode);
  }
};

BritepadApp* LifeApp::run() {

  switch (getAppMode()) {
    case MOUSE:
      mouse.run();
      // fall thorough

    case INTERACTIVE:
      if (pad.down(SCREEN_PAD) && generation) {
        wipe();
      }
      if (pad.touched(SCREEN_PAD)) {
        int x, y;
        if (dots.hit(pad.x(), pad.y(), &x, &y)) {
          dots.setDot(x,y, ONCOLOR);
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

  return STAY_IN_APP;
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
        color_t newcolor = nextgen[x][y] ? ONCOLOR : OFFCOLOR;
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
      reseed = true;
      nextRun = pad.time()+MILLIS_DELAY;
    } else {
      reseed = false;
      generation++;
    }
  }
}

void LifeApp::wipe(void) {
  dots.clear();
  generation = 0;
}

void LifeApp::seed(void) {
  wipe();
  for (int i = 0; i < 400; i++) {
    dots.setDot(random(DOTSWIDE), random(DOTSHIGH), ONCOLOR);
  }
}
