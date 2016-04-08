#include "BritepadShared.h"
#include "SparkleApp.h"
#include "Screen.h"
#include "Debug.h"
#include "Sound.h"

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
  } else {
    points[currPoint].x = -100;
    points[currPoint].y = -100;
  }

  currPoint++;
  if (currPoint >= pointCount) {
    currPoint = 0;
  }
}

