#include "BritepadShared.h"
#include "ScreensaverApp.h"

class BubblesApp : public ScreensaverApp {
  public:
    void run();
    const char* name() { return "Bubbles"; };

    appid_t id() { return ID; };
    static constexpr appid_t ID = "bubb";

    bool canBeMouse() { return true; }

  private:
    color_t currentColor = screen.red;
    coord_t lastX, lastY;
    coord_t deltaX, deltaY;
};

BubblesApp theBubblesApp;

void BubblesApp::run() {
  ScreensaverApp::run();
  int r;
  if (pad.pressed() || usbMouse.buttons()) {
    clearScreen();
  }
  if (((Uptime::millis() - usbMouse.lastMove()) < 500)) {
    lastX = usbMouse.x();
    lastY = usbMouse.y();
  } else if (pad.touched(SCREEN_PAD)) {
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
