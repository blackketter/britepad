#include "FastLED.h"
#include "BritepadShared.h"
#include "BubblesApp.h"
#include "Debug.h"

BubblesApp theBubblesApp;

void BubblesApp::run() {
  ScreensaverApp::run();
  int r;
  if (pad.down()) {
    clearScreen();
  }
  if (pad.touched(SCREEN_PAD)) {
    lastX = pad.x();
    lastY = pad.y();
  } else {
    deltaX = random(40)-20;
    deltaY = random(40)-20;
    lastX += deltaX;
    lastY += deltaY;
    if (lastY < screen.clipTop()) {
      lastY = screen.clipTop();
    }
    if (lastY > screen.clipBottom()) {
      lastY = screen.clipBottom();
    }
    if (lastX < screen.clipLeft()) {
      lastX = screen.clipLeft();
    }
    if (lastX > screen.clipRight()) {
      lastX = screen.clipRight();
    }
  }
  r = random(20);
  coord_t scatterX = random(40)-20;
  coord_t scatterY = random(40)-20;
  screen.fillCircle(lastX+scatterX, lastY+scatterY, r, currentColor);
  currentColor++;
}
