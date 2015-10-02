#include "FastLED.h"

#include "BritepadShared.h"
#include "BubblesApp.h"
#include "Debug.h"

BritepadApp* BubblesApp::run(void) {
  ScreensaverApp::run();
  coord_t x, y;
  int r;
  if (pad.down()) {
    clearScreen();
  }
  if (pad.touched(SCREEN_PAD)) {
    x = pad.x() + random(40)-20;
    y = pad.y() + random(40)-20;
    r = random(20);
  } else {
    x = random(screen.clipWidth());
    y = random(screen.clipHeight());
    r = random(40);
  }
  screen.fillCircle(x, y, r, currentColor);
  currentColor++;

  return STAY_IN_APP;
}
