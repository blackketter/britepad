#include "SplashApp.h"
#include "FastLED.h"

SplashApp theSplashApp;

void drawLogo(color_t c) {
  int weight = 20;
  int radius = 30;
  int ascender = radius*2;
  int midheight = screen.height() / 2;
  int midwidth = screen.width() / 2;
  screen.fillCircle(midwidth-radius, midheight, radius, c);
  screen.fillCircle(midwidth-radius, midheight, radius-weight, screen.black);
  screen.fillCircle(midwidth+radius, midheight, radius, c);
  screen.fillCircle(midwidth+radius, midheight, radius-weight, screen.black);
  screen.fillRect(midwidth-2*radius, midheight-2*radius, weight, ascender, c);
  screen.fillRect(midwidth, midheight, weight, ascender, c);
}

void SplashApp::begin(void) {
  drawindex = 0;
  screen.fillScreen(screen.black);
  currColor = screen.red;
  firstRun = millis();
//  drawLogo(currColor);
}


BritepadApp* SplashApp::run(void) {
  if (millis() - firstRun > splashDuration) {
    return SCREENSAVER_APP;
  }

  int weight = 18;
  int loopradius = screen.height()/10;
  int ascender = loopradius/2*5;
  int midheight = screen.height() / 2;
  int midwidth = screen.width() / 2;
  int circlesteps = 128;
  int totalsteps = circlesteps+ascender;
  // 256 steps to draw the circles
  // ascender steps to top/bottom of ascender/descender
  if (drawindex < circlesteps) {
    long theta = drawindex*65536/circlesteps-65536/4;
    coord_t x1 =  loopradius*((long)sin16(theta))/32768;
    coord_t y1 =  loopradius*((long)cos16(theta))/32768;
    screen.fillCircle(midwidth-weight/2-loopradius+x1,midheight - y1, weight/2, screen.red);
    screen.fillCircle(midwidth+weight/2+loopradius+x1,midheight + y1, weight/2, screen.red);
//    screen.drawLine(midwidth-radius + x1,midheight - y1,midwidth-radius+x2, midheight- y2,screen.red);
//    screen.drawLine(midwidth+radius + x1,midheight + y1,midwidth+radius+x2, midheight+ y2,screen.red);
  } else if (drawindex <= totalsteps) {
    int yoffset = drawindex - circlesteps;
    screen.fillCircle(midwidth-loopradius*2-weight/2,midheight - yoffset, weight/2, screen.red);
    screen.fillCircle(midwidth+weight/2,midheight + yoffset, weight/2, screen.red);

//    screen.drawLine(midwidth-3*weight, midheight-yoffset, midwidth-2*weight, midheight-yoffset, screen.red);
//    screen.drawLine(midwidth, midheight+yoffset, midwidth+weight, midheight+yoffset, screen.red);
  } else if (drawindex == totalsteps+1) {
//    drawLogo(currColor);
  }

  if (drawindex <= totalsteps+1) {
    drawindex++;
  }

  return STAY_IN_APP;
}
