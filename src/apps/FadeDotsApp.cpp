#include "FadeDotsApp.h"

FadeDotsApp theFadeDotsApp;

void FadeDotsApp::run() {
  ScreensaverApp::run();
  if (pad.touched(SCREEN_PAD)) {
    int x, y;
    if (dots->hit(pad.x(), pad.y(), &x, &y)) {
      dots->setDot(x,y, dots->getDot(x,y)+0x000f);
      dots->updateDot(x,y);
    }
  }

  int x= random(dots->getDotsWide());
  int y= random(dots->getDotsHigh());
  dots->setDot(x,y, dots->getDot(x,y)+1);
  dots->updateDot(x,y);
}

