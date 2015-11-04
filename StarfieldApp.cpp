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

void StarfieldApp::setDirection(direction_t newDirection) {
  if (newDirection == direction && stars && distance) {
    return;
  }

  direction = newDirection;
  releaseMem();

  if (newDirection == DIRECTION_UP || newDirection == DIRECTION_DOWN) {
    starCount = screen.clipWidth();
    starRange = screen.clipHeight();
  } else {
    starCount = screen.clipHeight();
    starRange = screen.clipWidth();
  }

  stars = new coord_t[starCount];
  distance = new uint8_t[starCount];

  if (stars && distance) {
    for (int i = 0; i < starCount; i++) {
      stars[i] = random(starRange);
      distance[i] = randomDistance();
    }
  }
}

void StarfieldApp::begin() {
  ScreensaverApp::begin();
  setDirection((direction_t)random(4));
}

void StarfieldApp::run() {
  ScreensaverApp::run();

  if (pad.touched(SCREEN_PAD)) {
    int circleSize = 20;
    screen.fillCircle(pad.x(), pad.y(), circleSize, screen.darkergrey);
  }
  if (direction == DIRECTION_LEFT) {
    for (int i = screen.clipTop(); i < screen.clipBottom(); i++) {
      screen.drawLine(stars[i], i, stars[i]+distance[i],i,screen.black);
      stars[i]-= distance[i];
      if (stars[i]<screen.clipLeft()) {
        stars[i] = screen.clipRight();
        distance[i] = randomDistance();
      }
      screen.drawLine(stars[i]+distance[i], i, stars[i], i, screen.white);
    }
  } else if (direction == DIRECTION_RIGHT) {
    for (int i = screen.clipTop(); i < screen.clipBottom(); i++) {
      screen.drawLine(stars[i], i, stars[i]+distance[i],i,screen.black);
      stars[i]+= distance[i];
      if (stars[i]>screen.clipRight()) {
        stars[i] = screen.clipLeft();
        distance[i] = randomDistance();
      }
      screen.drawLine(stars[i]+distance[i], i, stars[i], i, screen.white);
    }

  } else if (direction == DIRECTION_UP) {
    for (int i = screen.clipLeft(); i < screen.clipRight(); i++) {
      screen.drawLine(i, stars[i], i, stars[i]+distance[i], screen.black);
      stars[i]-= distance[i];
      if (stars[i]<screen.clipTop()) {
        stars[i] = screen.clipBottom();
        distance[i] = randomDistance();
      }
      screen.drawLine(i, stars[i]+distance[i], i, stars[i], screen.white);
    }

  } else  {  // down
    for (int i = screen.clipLeft(); i < screen.clipRight(); i++) {
      screen.drawLine(i, stars[i], i, stars[i]+distance[i], screen.black);
      stars[i]+= distance[i];
      if (stars[i]>screen.clipBottom()) {
        stars[i] = screen.clipTop();
        distance[i] = randomDistance();
      }
      screen.drawLine(i, stars[i]+distance[i], i, stars[i], screen.white);
    }
  }
}
