#include "BritepadShared.h"

#include "MandelApp.h"
#include "Screen.h"
#include "Sound.h"

MandelApp theMandelApp;

void MandelApp::begin() {
  ScreensaverApp::begin();
  xPixel = screen.clipRight()+1; // force a restart
  _column = new color_t[screen.clipHeight()];
  restart();
}

void MandelApp::end() {
  delete _column;
}

void MandelApp::restart() {
  xOrigin = (float)rand()/(float)(RAND_MAX/2)-1.0;
  yOrigin = (float)rand()/(float)(RAND_MAX/2)-1.0;
  scale = 1.0f;
  pixelSum = 0;
  lastPixelSum = 1;
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
      // restart if the screen is the same or we have zoomed in too far
      if (scale <= 0.00001f || (lastPixelSum == pixelSum)) {
        restart();
      } else {
        lastPixelSum = pixelSum;
        pixelSum = 0;
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
    color_t pixel = ((iter << 7 & 0xF8) << 8) | ((iter << 4 & 0xFC) << 3) | (iter >> 3);
    pixelSum += pixel;
    _column[yPixel] = pixel;
    yPixel++;
  }
}
