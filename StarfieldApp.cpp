#include "FastLED.h"

#include "BritepadShared.h"
#include "StarfieldApp.h"
#include "Debug.h"

void StarfieldApp::releaseMem() {
  if (stars) {
    delete [] stars;
    stars = nullptr;
  }

  if (distance) {
    delete [] distance;
    distance = nullptr;
  }
}

void StarfieldApp::run() {
  ScreensaverApp::run();

  if (!stars || !distance) {
    releaseMem();
    stars = new coord_t[screen.clipHeight()];
    distance = new uint8_t[screen.clipHeight()];

    if (stars && distance) {
      for (int i = 0; i < screen.clipHeight(); i++) {
        stars[i] = random(screen.clipWidth());
        distance[i] = randomDistance();
      }
    }
  }

  if (pad.touched(SCREEN_PAD)) {
    int circleSize = 20;
    screen.fillCircle(pad.x(), pad.y(), circleSize, screen.darkergrey);
  }

  for (int i = screen.clipTop(); i < screen.clipBottom(); i++) {
    screen.drawLine(stars[i], i, stars[i]+distance[i],i,screen.black);
    stars[i]-= distance[i];
    if (stars[i]<screen.clipLeft()) {
      stars[i] = screen.clipRight();
      distance[i] = randomDistance();
    }
    screen.drawLine(stars[i]+distance[i], i, stars[i], i, screen.white);
  }
}
