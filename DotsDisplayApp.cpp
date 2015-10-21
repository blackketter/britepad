#include "FastLED.h"

#include "BritepadShared.h"
#include "DotsDisplayApp.h"
#include "Debug.h"


void DotsDisplayApp::run(void) {
  ScreensaverApp::run();

  if (pad.touched(SCREEN_PAD)) {
    int x, y;
    if (dots.hit(pad.x(), pad.y(), &x, &y)) {
      dots.setDot(x,y, lastColor);
      dots.updateDot(x,y);
    }
  } else {
    int x = random(dots.getDotsWide());
    int y = random(dots.getDotsHigh());
    lastColor = random(screen.white);
    dots.setDot(x, y, lastColor);
    dots.updateDot(x, y);
  }
}
