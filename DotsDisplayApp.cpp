#include "FastLED.h"

#include "BritepadShared.h"
#include "DotsDisplayApp.h"
#include "Debug.h"

void DotsDisplayApp::updateDot(int x, int y) {
  int dotspacing_w = screen.width() / dots_wide;
  int dotspacing_h = screen.height() / dots_high;
  int r = min((screen.width() * 8 / dots_wide) / 10 / 2, (screen.height() * 8 / dots_high) / 10 / 2);  // dots are 80% of space
  int center_x = x * dotspacing_w + dotspacing_w/2;
  int center_y = y * dotspacing_h + dotspacing_h/2;
  screen.fillCircle(center_x, center_y, r, dots[x*dots_wide+y]);
}

void DotsDisplayApp::updateDots() {

  for (int x = 0; x < dots_wide; x++) {
    for (int y = 0; y < dots_high; y++) {
      updateDot(x,y);
    }
  }
}

void DotsDisplayApp::setDot(int x, int y, color_t c) {
  dots[x*dots_wide+y] = c;
  updateDot(x,y);
}

void DotsDisplayApp::writeDot(int x, int y, color_t c) {
  dots[x*dots_wide+y] = c;
}

void DotsDisplayApp::setUpDots(int width, int height) {
    dots_wide = width;
    dots_high = height;
    dots = (color_t*)BritepadAppScratchPad;
}

BritepadApp* DotsDisplayApp::run(void) {

//  updateDots();
  setDot(random(dots_wide), random(dots_high), random(65535));

  return STAY_IN_APP;
}

