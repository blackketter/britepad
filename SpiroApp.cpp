#include "BritepadShared.h"
#include "SpiroApp.h"
#include "Debug.h"

void SpiroApp::begin() {
  ScreensaverApp::begin();
  reset();
}

void SpiroApp::reset() {
  R = random(random(20))+1;
  r = random(random(20))+1;
  d = random(random(20))+1;
  scale = (float)(screen.clipHeight())/2/(abs(R-r)+d);
  scale = scale*(random(10)+1)/10;  // randomly smaller by a factor of 0.1-1.0
  center.x = screen.clipMidWidth();
  center.y = screen.clipMidHeight();
  currColor = screen.brighten(random(65536));
  theta = 0.0;
  dTheta = 2*3.14159/200;
  endTheta = 2*3.14159 * r;
  first = true;
}

void SpiroApp::run() {
  ScreensaverApp::run();

  point_t p;
  // Hypotrochoid
  p.x = scale*((R - r)*cos(theta) + d*cos(theta*(R-r)/r));
  p.y = scale*((R - r)*sin(theta) - d*sin(theta*(R-r)/r));

  p.x += center.x;
  p.y += center.y;

  if (first) {
    first = false;
  } else {
    screen.drawLine(lastP.x, lastP.y, p.x,p.y, currColor);
  }

  lastP = p;

  theta += dTheta;

  if (theta > endTheta) {
    iters++;
    if (iters > 5) {
      clearScreen();
      iters = 0;
    }
    reset();
  }
}

