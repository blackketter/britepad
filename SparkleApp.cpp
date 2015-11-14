#include "BritepadShared.h"
#include "SparkleApp.h"
#include "Screen.h"
#include "Debug.h"
#include "Sound.h"

void SparkleApp::run() {
  BritepadApp::run();

  for (int i = 0; i < pointCount; i++) {
    int distance = i + pointCount - currPoint + 1;
    uint8_t brightness = (distance*255)/pointCount;
    color_t sparklecolor = grey8toC16(brightness);
    screen.fillRect(points[i].x,points[i].y,2,2,sparklecolor);
    distance++;
  }

  if (pad.touched(SCREEN_PAD)) {
    points[currPoint].x = pad.x() + random(30) - 15;
    points[currPoint].y = pad.y() + random(30) - 15;
    screen.fillRect(points[currPoint].x, points[currPoint].y, 2,2,screen.white);
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

