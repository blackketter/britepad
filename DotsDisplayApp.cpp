#include "FastLED.h"

#include "BritepadShared.h"
#include "DotsDisplayApp.h"
#include "Debug.h"

void DotsDisplayApp::updateDot(int x, int y) {
  int dotspacing_w = screen.width() / pixels_wide;
  int dotspacing_h = screen.height() / pixels_high;
  int r = min((screen.width() * 8 / pixels_wide) / 10 / 2, (screen.height() * 8 / pixels_high) / 10 / 2);  // dots are 80% of space
  int center_x = x * dotspacing_w + dotspacing_w/2;
  int center_y = y * dotspacing_h + dotspacing_h/2;
  screen.fillCircle(center_x, center_y, r, pixels[x*pixels_wide+y]);
}

void DotsDisplayApp::updateDots() {

  for (int x = 0; x < pixels_wide; x++) {
    for (int y = 0; y < pixels_high; y++) {
      updateDot(x,y);
    }
  }
}

void DotsDisplayApp::setDot(int x, int y, color_t c) {
  pixels[x*pixels_wide+y] = c;
  updateDot(x,y);
}

void DotsDisplayApp::writeDot(int x, int y, color_t c) {
  pixels[x*pixels_wide+y] = c;
}

void DotsDisplayApp::setUpDots(int width, int height) {
    pixels_wide = width;
    pixels_high = height;
    pixels = (color_t*)BritepadAppScratchPad;
}

BritepadApp* DotsDisplayApp::run(void) {

//  updateDots();
  setDot(random(pixels_wide), random(pixels_high), random(65535));

  return STAY_IN_APP;
}

