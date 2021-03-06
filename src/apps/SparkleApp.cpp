#include "BritepadShared.h"
#include "Screen.h"
#include "Sound.h"
#include "BritepadApp.h"

class SparkleApp : public BritepadApp {
  public:
    void run();

    bool canBeMouse() { return true; }
    bool canBeInteractive() { return true; }
    bool canBeScreensaver() { return true; }
    bool disablesScreensavers() { return getAppMode() == INTERACTIVE_MODE; }

    AppType getAppType() { return (AppType)(MOUSE_APP + INTERACTIVE_APP + SCREENSAVER_APP); }

    const char* name() { return "Sparkle"; };
    appid_t id() { return ID; };
    static constexpr appid_t ID = "spkl";

  private:
    void drawSparkle(int i, color_t c);
    static const int pointCount = 100;
    point_t points[pointCount];
    int currPoint = 0;
    static const int spread = 60;
    static const int radius = 4;
};

SparkleApp theSparkleApp;

void SparkleApp::drawSparkle(int i, color_t c) {
   screen.drawLine(points[i].x-radius, points[i].y, points[i].x + radius, points[i].y, c);
   screen.drawLine(points[i].x, points[i].y-radius, points[i].x, points[i].y+radius, c);
}

void SparkleApp::run() {
  BritepadApp::run();

  for (int i = 0; i < pointCount; i++) {
    int distance = i + pointCount - currPoint + 1;
    uint8_t brightness = (distance*255)/pointCount;
    if (brightness) {  brightness = random(brightness); }
    drawSparkle(i, screen.black);
    if (random(2)) { points[i].y++; }
    color_t sparklecolor = grey8toC16(brightness);
    drawSparkle(i, sparklecolor);
    distance++;
  }

  if (pad.touched(SCREEN_PAD)) {
    points[currPoint].x = pad.x() + random(spread) - spread/2;
    points[currPoint].y = pad.y() + random(spread) - spread/2;
    drawSparkle(currPoint, screen.white);
    // todo: play a sparkly sound
  } else if ((Uptime::millis() - usbMouse.lastMove()) < 500) {
    points[currPoint].x = usbMouse.x() + random(spread) - spread/2;
    points[currPoint].y = usbMouse.y() + random(spread) - spread/2;
    drawSparkle(currPoint, screen.white);
    // todo: play a sparkly sound
  } else {
    points[currPoint].x = -100;
    points[currPoint].y = -100;
  }

  currPoint++;
  if (currPoint >= pointCount) {
    currPoint = 0;
  }
}

