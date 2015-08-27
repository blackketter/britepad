#include "SplashApp.h"

void drawLogo(void) {
  int weight = 20;
  int radius = 30;
  int ascender = radius*2;
  int midheight = screen.height() / 2;
  int midwidth = screen.width() / 2;
  screen.fillScreen(screen.black);
  screen.fillCircle(midwidth-radius, midheight, radius, screen.red);
  screen.fillCircle(midwidth-radius, midheight, radius-weight, screen.black);
  screen.fillCircle(midwidth+radius, midheight, radius, screen.red);
  screen.fillCircle(midwidth+radius, midheight, radius-weight, screen.black);
  screen.fillRect(midwidth-2*radius, midheight-2*radius, weight, ascender, screen.red);
  screen.fillRect(midwidth, midheight, weight, ascender, screen.red);
}

void SplashApp::begin(void) {
  drawLogo();
}

BritepadApp* SplashApp::run(void) {
  // exit if any pad is touched
  if (pad.touched(ANY_PAD)) {
    return DEFAULT_APP;
  } else {
    return nil;
  }
}
