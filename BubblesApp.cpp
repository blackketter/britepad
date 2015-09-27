#include "FastLED.h"

#include "BritepadShared.h"
#include "BubblesApp.h"
#include "Debug.h"

BritepadApp* BubblesApp::run(void) {
  coord_t x = random(screen.clipWidth());
  coord_t y = random(screen.clipHeight());
  int r = random(40);
  screen.fillCircle(x, y, r, currentColor);
  currentColor++;

  return STAY_IN_APP;
}
