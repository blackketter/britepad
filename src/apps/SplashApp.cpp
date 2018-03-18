#include "SplashApp.h"

SplashApp theSplashApp;

void drawLogo(color_t c) {
  int weight = 20;
  int radius = 30;
  int ascender = radius*2;
  int midheight = screen.clipHeight() / 2;
  int midwidth = screen.clipWidth() / 2;
  screen.fillCircle(midwidth-radius, midheight, radius, c);
  screen.fillCircle(midwidth-radius, midheight, radius-weight, screen.black);
  screen.fillCircle(midwidth+radius, midheight, radius, c);
  screen.fillCircle(midwidth+radius, midheight, radius-weight, screen.black);
  screen.fillRect(midwidth-2*radius, midheight-2*radius, weight, ascender, c);
  screen.fillRect(midwidth, midheight, weight, ascender, c);
}

void SplashApp::begin(AppMode asMode) {
  BritepadApp::begin(asMode);
  drawindex = 0;
  clearScreen();
  currColor = screen.red;
  firstRun = Uptime::millis();
//  drawLogo(currColor);
}


void SplashApp::run() {
  if (Uptime::millis() - firstRun > splashDuration) {
    launchApp(A_SCREENSAVER_APP, SCREENSAVER_MODE);
  }

  int weight = 18;
  int loopradius = screen.clipHeight()/10;
  int ascender = loopradius/2*5;
  int midheight = screen.clipHeight() / 2;
  int midwidth = screen.clipWidth() / 2;
  int circlesteps = 128;
  int totalsteps = circlesteps+ascender;
  // 256 steps to draw the circles
  // ascender steps to top/bottom of ascender/descender
  if (drawindex < circlesteps) {
    int32_t theta = drawindex*65536/circlesteps-65536/4;
    coord_t x1 =  loopradius*((int32_t)sin16(theta))/32768;
    coord_t y1 =  loopradius*((int32_t)cos16(theta))/32768;
    screen.fillCircle(midwidth-weight/2-loopradius+x1,midheight - y1, weight/2, screen.red);

// for BP britepad
//    screen.fillCircle(midwidth+weight/2+loopradius+x1,midheight + y1, weight/2, screen.red);

// for BB briteboard
    screen.fillCircle(midwidth+weight/2+loopradius+x1, midheight - y1, weight/2, screen.red);

  } else if (drawindex <= totalsteps) {
    int yoffset = drawindex - circlesteps;
    screen.fillCircle(midwidth-loopradius*2-weight/2,midheight - yoffset, weight/2, screen.red);

// for britepad
//    screen.fillCircle(midwidth+weight/2,midheight + yoffset, weight/2, screen.red);

// for briteboard
    screen.fillCircle(midwidth+weight/2,midheight - yoffset, weight/2, screen.red);

  } else if (drawindex >= totalsteps+1) {
//    drawLogo(currColor);
  }

  if (drawindex <= totalsteps+1) {
    drawindex++;
  }
}
