#include "BritepadShared.h"
#include "DotsDisplayApp.h"

DotsDisplayApp theDotsDisplayApp;

void DotsDisplayApp::begin(AppMode asMode) {

  if (!dots) {
    dots = new DotMatrix(screen.clipLeft(), screen.clipTop(), screen.clipWidth(), screen.clipHeight(), getDotsWide(), getDotsHigh());
  }

  ScreensaverApp::begin(asMode);
};

void DotsDisplayApp::end() {
   if (dots) {
    delete dots;
    dots = nullptr;
  }
  ScreensaverApp::end();
}

void DotsDisplayApp::run() {

  ScreensaverApp::run();

  int x, y;
  bool hit = true;

  if ((Uptime::millis() - usbMouse.lastMove()) < 500) {
    hit = dots->hit(usbMouse.x(), usbMouse.y(), &x, &y);
  } else if (pad.touched(SCREEN_PAD)) {
    hit = dots->hit(pad.x(), pad.y(), &x, &y);
  } else {
    x = random(dots->getDotsWide());
    y = random(dots->getDotsHigh());
    lastColor = random(screen.white);
  }
  if (hit) {
    dots->setDot(x, y, lastColor);
    dots->updateDot(x, y);
  }
}
