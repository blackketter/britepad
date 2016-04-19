#include "BritepadShared.h"
#include "MandelApp.h"
#include "Screen.h"
#include "Debug.h"
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
  color_t color;

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

  float cx = xPixel * sx / screen.clipWidth() + x1;

  for (yPixel = screen.clipTop(); yPixel < screen.clipBottom(); yPixel++) {
    float cy = yPixel * sy / screen.clipHeight() + y1;
    float x = 0.0f, y = 0.0f, xx = 0.0f, yy = 0.0f;
    for (iter = 0; iter <= MAX && (xx + yy) < 4.0f; iter++) {
      xx = x * x;
      yy = y * y;
      y = 2.0f * x * y + cy;
      x = xx - yy + cx;
    }
    color = ((iter << 7 & 0xF8) << 8) | ((iter << 4 & 0xFC) << 3) | (iter >> 3);
    screen.drawPixel(xPixel, yPixel, color);
  }

  xPixel++;
}
