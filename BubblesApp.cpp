#include <Time.h>
#include "FastLED.h"

#include "BritepadShared.h"
#include "BubblesApp.h"
#include "Debug.h"

BritepadApp* BubblesApp::run(void) {
  int x = random(screen.width());
  int y = random(screen.height());
  long r = random(40);
  screen.fillCircle(x, y, r, currentColor);
  currentColor++;

  return nil;
}
