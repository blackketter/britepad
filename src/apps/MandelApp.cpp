#include "BritepadShared.h"
#include "MandelApp.h"
#include "Screen.h"
#include "Sound.h"

MandelApp theMandelApp;

void MandelApp::begin(AppMode asMode) {
  ScreensaverApp::begin(asMode);
  xPixel = screen.clipRight()+1; // force a restart
}


void MandelApp::run() {
  ScreensaverApp::run();

  const int MAX = 128;
  int iter;

  if (xPixel > screen.clipRight()) {
    xPixel = screen.clipLeft();
    scale *= 0.7f;
    if (scale <= 0.00001f) {
      scale = 1.0f;
    }

    x1 = -2.0f * scale + xOrigin;
    y1 = -1.5f * scale + yOrigin;
    x2 = 2.0f * scale + xOrigin;
    y2 = 1.5f * scale + yOrigin;
    sy = y2 - y1;
    sx = x2 - x1;
  }

  int16_t w = screen.clipWidth();
  int16_t h = screen.clipHeight();

  float cx = xPixel * sx / w + x1;

  color_t column[h];

  for (yPixel = 0; yPixel < h; yPixel++) {
    float cy = yPixel * sy / h + y1;
    float x = 0.0f, y = 0.0f, xx = 0.0f, yy = 0.0f;
    for (iter = 0; iter <= MAX && (xx + yy) < 4.0f; iter++) {
      xx = x * x;
      yy = y * y;
      y = 2.0f * x * y + cy;
      x = xx - yy + cx;
    }
    column[yPixel] = ((iter << 7 & 0xF8) << 8) | ((iter << 4 & 0xFC) << 3) | (iter >> 3);
  }
  screen.drawFastVLine(xPixel,0,h,column);
  xPixel++;
}
