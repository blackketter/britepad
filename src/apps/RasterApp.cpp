#include "BritepadShared.h"
#include "RasterApp.h"
#include "Screen.h"
#include "Sound.h"

RasterApp theRasterApp;

void RasterApp::begin(AppMode asMode) {
  ScreensaverApp::begin(asMode);
}


void RasterApp::run() {
  ScreensaverApp::run();
  coord_t w = screen.clipWidth();
  coord_t t = screen.clipTop();
  coord_t b = screen.clipBottom();
  coord_t l = screen.clipLeft();
  coord_t r = screen.clipRight();

  color_t row[w];

  color_t c = random(65536);

  for (int y = t; y < b; y++) {
    for (int x = l; x < r; x++) {
      row[x] = c++;
    }
    if (y%10 == 0) {
      launcher.idle();
    }
    screen.drawFastHLine(l, y, w, row);
  }
}
