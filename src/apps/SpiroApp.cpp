#include "BritepadShared.h"
#include "SpiroApp.h"

SpiroApp theSpiroApp;

void SpiroApp::begin(AppMode asMode) {
  ScreensaverApp::begin(asMode);
  reset();
}

void SpiroApp::reset() {
  R = random(random(10))+11;

  // doesn't work when they are the same
  do {
    r = random(random(20))+1;
  } while (r == R);

  d = random(random(20))+1;

  scale = (float)(screen.clipHeight())/2/(abs(R-r)+d);
  scale = scale*(random(4)+7)/10;  // randomly smaller by a factor of 0.3-1.0
  center.x = screen.clipMidWidth();
  center.y = screen.clipMidHeight();
  currColor = random(0x10000) | random(0x10000);
  float thetaOffset = 2*3.14159*3/4;
  theta = 0.0 + thetaOffset;
  dTheta = 2*3.14159/200;
  endTheta = 2*3.14159 * r + thetaOffset * r;
  if (iters == maxSpiros - 1) {
    endTheta *= 3;  // draw the last one thrice to keep it on the screen
  }

  first = true;
}

void SpiroApp::drawInfo() {
    screen.setCursor(screen.clipLeft(), screen.clipTop());
    screen.setFont(Arial_8_Bold);
    screen.setTextColor(screen.red, bgColor());
    if (displayInfo) {
      screen.drawTextF("R: %d  \nr: %d  \nd: %d   ", R,r,d);
    } else {
      screen.drawText("              \n");
      screen.drawText("              \n");
      screen.drawText("              \n");
    }
}

void SpiroApp::run() {
  ScreensaverApp::run();

  if (pad.pressed(BOTTOM_PAD)) {
    displayInfo = !displayInfo;
    drawInfo();
  }

  point_t p;
  // Hypotrochoid
  p.x = scale*((R - r)*cosf(theta) + d*cosf(theta*(R-r)/r));
  p.y = scale*((R - r)*sinf(theta) - d*sinf(theta*(R-r)/r));

  p.x += center.x;
  p.y += center.y;

  if (first) {
    first = false;
    drawInfo();
  } else {
    screen.drawLine(lastP.x, lastP.y, p.x,p.y, currColor);
  }

  lastP = p;


  if (theta > endTheta) {
    iters++;
    if (iters >= maxSpiros) {
      clearScreen();
      iters = 0;
    }
    reset();
  } else {
    theta += dTheta;
  }
}

