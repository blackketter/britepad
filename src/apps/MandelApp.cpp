#include "BritepadShared.h"
#include "MandelApp.h"
#include "Screen.h"
#include "Sound.h"

MandelApp theMandelApp;

void MandelApp::begin(AppMode asMode) {
  ScreensaverApp::begin(asMode);
  xPixel = screen.clipRight()+1; // force a restart
  _column = new color_t[screen.clipHeight()];
}

void MandelApp::end() {
  delete _column;
}

void MandelApp::run() {
  ScreensaverApp::run();

  const int MAX = 128;
  int iter;
  int16_t w = screen.clipWidth();
  int16_t h = screen.clipHeight();
  int16_t r = screen.clipRight();
  int16_t l = screen.clipLeft();

  while (!timeToLeave()) {
    if (yPixel >= h) {
      screen.drawFastVLine(xPixel,0,h,_column);
      yPixel = 0;
      xPixel++;
    }

    if (xPixel > r) {
      xPixel = l;
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

    float cx = xPixel * sx / w + x1;
    float cy = yPixel * sy / h + y1;

    float x = 0.0f, y = 0.0f, xx = 0.0f, yy = 0.0f;

    for (iter = 0; iter <= MAX && (xx + yy) < 4.0f; iter++) {
      xx = x * x;
      yy = y * y;
      y = 2.0f * x * y + cy;
      x = xx - yy + cx;
    }
    _column[yPixel] = ((iter << 7 & 0xF8) << 8) | ((iter << 4 & 0xFC) << 3) | (iter >> 3);
    yPixel++;
  }
}
