#include "FadeDotsApp.h"

BritepadApp* FadeDotsApp::run() {
  ScreensaverApp::run();
  if (pad.touched(SCREEN_PAD)) {
    int x, y;
    if (dots.hit(pad.x(), pad.y(), &x, &y)) {
      dots.setDot(x,y, dots.getDot(x,y)+0x000f);
      dots.updateDot(x,y);
    }
  }

  int x= random(dots.getDotsWide());
  int y= random(dots.getDotsHigh());
  dots.setDot(x,y, dots.getDot(x,y)+1);
  dots.updateDot(x,y);
  return STAY_IN_APP;
}


void FadeDotsApp::begin(AppMode asMode) {
  ScreensaverApp::begin(asMode);
  dots.init(16,12,BritepadAppScratchPad);
};
