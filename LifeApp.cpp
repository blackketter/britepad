#include "LifeApp.h"

BritepadApp* LifeApp::run() {
  int x= random(dots.getDotsWide());
  int y= random(dots.getDotsHigh());
  dots.setDot(x,y, dots.getDot(x,y)+1);
  dots.updateDot(x,y);
  return STAY_IN_APP;
}


void LifeApp::begin(AppMode asMode) {
  DEBUG_LN("begin LifeApp");
  ScreensaverApp::begin(asMode);
  dots.init(16,12,BritepadAppScratchPad);
};
