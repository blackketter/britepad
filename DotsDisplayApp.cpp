#include "FastLED.h"

#include "BritepadShared.h"
#include "DotsDisplayApp.h"
#include "Debug.h"


BritepadApp* DotsDisplayApp::run(void) {
  ScreensaverApp::run();
  int x = random(dots.getDotsWide());
  int y = random(dots.getDotsHigh());
  dots.setDot(x, y, random(65535));
  dots.updateDot(x, y);
  return STAY_IN_APP;
}
