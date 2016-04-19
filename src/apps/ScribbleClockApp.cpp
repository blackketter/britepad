#include "ScribbleClockApp.h"
// Using the default gesture data for scribble shapes
#include "Gesture.h"

ScribbleClockApp theScribbleClockApp;

void ScribbleClockApp::update() {
    // first column is 10s of hours
    int h10 = clock.hourFormat12() / 10;
//    h10 = clock.second()%10;
    int h1 = clock.hourFormat12() % 10;
    int m10 = clock.minute() / 10;
    int m1 = clock.minute() % 10;
    int s10 = clock.second() / 10;
    int s1 = clock.second() % 10;

    int digitsDrawn;

    if (lastH10 != h10) {
      lastH10 = lastH1 = lastM10 = lastM1 = lastS10 = lastS1 = 11;
      clearScreen();
      colons = false;
    }

    if (h10 == 0) {
      digitsDrawn = 7; // including the colon
    } else {
      digitsDrawn = 8;
    }

    coord_t w = screen.clipWidth()/digitsDrawn;
    coord_t h = screen.clipHeight()/3;
    coord_t y = h+screen.clipTop();
    coord_t x = screen.clipLeft() + 5;

    rect_t r = {x,y,(coord_t)(w-10),h};  // digits are spaced out by 10 pixels

    if (h10 != 0) {
      // supress leading digit
      if (lastH10 != h10) {
        drawDigit(h10,r);
      }
      r.x += w;
    }
    lastH10 = h10;

    if (lastH1 != h1) {
      drawDigit(h1,r);
      lastH1 = h1;
    }
    r.x += w;

    rect_t dots;
    if (!colons){
      // The colon is just couple of small zeros
      dots = { (coord_t)(r.x + r.w/3), (coord_t)(r.y+r.h/6), (coord_t)(r.w/3), (coord_t)(r.h/6)};
      drawDigit(0, dots);
      dots.y+=r.h/3;
      drawDigit(0, dots);
    }

    r.x += w;

    if (lastM10 != m10) {
      drawDigit(m10,r);
      lastM10 = m10;
    }
    r.x += w;

    if (lastM1 != m1) {
      drawDigit(m1,r);
      lastM1 = m1;
    }
    r.x += w;

    if (!colons) {
      dots = { (coord_t)(r.x + r.w/3), (coord_t)(r.y+r.h/6), (coord_t)(r.w/3), (coord_t)(r.h/6)};
      drawDigit(0, dots);
      dots.y+=r.h/3;
      drawDigit(0, dots);
    }
    colons = true;

    r.x += w;
    if (lastS10 != s10) {
      drawDigit(s10,r);
      lastS10 = s10;
    }

    r.x += w;
    if (lastS1 != s1) {
      drawDigit(s1,r);
      lastS1 = s1;
    }

}

void ScribbleClockApp::drawDigit(int digit, rect_t& r) {
  int i = 0;

  screen.fillRectx(r, screen.black);

  while (defaultGestures[i].gesture != 0) {
    if (defaultGestures[i].gesture == digit + '0') {
      // rotate the digit:
      int jitter = digit == 0 ? 256 : 20;
      float by = - M_PI/2.0 + (defaultGestures[i].shape->orientation + random(jitter) - jitter/2) / 256.0 * M_PI * 2.0;
      float sinOrientation = sinf(by);
      float cosOrientation = cosf(by);
      pointf_t rotated[samplesPerGesture];
      for (int j = 0; j < samplesPerGesture; j++) {
        coord_t x = ((coord_t)defaultGestures[i].shape->points[j].x - 128);
        coord_t y = ((coord_t)defaultGestures[i].shape->points[j].y - 128);
        rotated[j].x = x*cosOrientation - y*sinOrientation;
        rotated[j].y = x*sinOrientation + y*cosOrientation;
      }

      // put it in unit square
      float minx = 0;
      float miny = 0;
      float maxx = 0;
      float maxy = 0;

      for (int j = 0; j < samplesPerGesture; j++) {
        if (rotated[j].x < minx) {
          minx = rotated[j].x;
        }
        if (rotated[j].x > maxx) {
          maxx = rotated[j].x;
        }
        if (rotated[j].y < miny) {
          miny = rotated[j].y;
        }
        if (rotated[j].y > maxy) {
          maxy = rotated[j].y;
        }
      }

      float scale = max(maxx-minx,maxy-miny);

//      DEBUGF("minx: %f miny: %f maxx: %f maxy: %f\n",minx,miny,maxx,maxy);
      point_t lastPoint = {0,0};
      for (int j = 0; j < samplesPerGesture; j++) {
        point_t currPoint = {0,0};
        float x, y;
        x = (rotated[j].x - minx)/scale;
        y = (rotated[j].y - miny)/scale;
//        DEBUGF("x: %f, y: %f\n",x,y);
        currPoint.x = r.x + ((r.w-1) * x);
        currPoint.y = r.y + ((r.h-1) * y);

        if (j != 0) {
          screen.drawLinex(lastPoint, currPoint, screen.yellow);
        } else {
//          screen.fillCircle(currPoint.x, currPoint.y, 4, screen.green);
        }
        lastPoint.x = currPoint.x;  lastPoint.y = currPoint.y;
      }
      break;
    }
    i++;
  }
}
