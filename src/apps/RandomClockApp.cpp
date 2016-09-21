#include "BritepadShared.h"
#include "RandomClockApp.h"
#include "Debug.h"
#include "Clock.h"

RandomClockApp theRandomClockApp;

void RandomClockApp::begin(AppMode asMode) {
  ClockApp::begin(asMode);
}

float distance(long x1, long y1, long x2, long y2) {
  return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}

float onLine(long x1, long y1, long x2, long y2, long x, long y) {
  float d = distance(x1,y1,x2,y2);
  float d1 = distance(x1,y1,x,y);
  float d2 = distance(x2,y2,x,y);
  return ((d1+d2)-d)/d;
}

void RandomClockApp::update() {
    coord_t x, y;
    coord_t xCenter = screen.clipMidWidth();
    coord_t yCenter = screen.clipMidHeight();
    coord_t xLine;
    coord_t yLine;
    float dist;
    long r;
    uint16_t hr_theta, min_theta, sec_theta, millis_theta;

    uint8_t now_sec = clock.second();
    uint16_t now_millis = clock.fracMillis();

    millis_theta = (now_millis * 65536L)/1000L;

    sec_theta = ((int32_t)now_sec * 65536L)/60L + millis_theta/60;

    min_theta = (clock.minute() * 65536L)/60L + sec_theta/60;

    int32_t hr  = clock.hourFormat12();
    if (hr==12) { hr=0; }

    hr_theta = (hr * 65536L) / 12L + min_theta/12;

    for (int p = 0; p < 10; p++) {
      int i = 0;
      do {
        i++;
        x = random(screen.clipWidth()) + screen.clipLeft();
        y = random(screen.clipHeight()) + screen.clipTop();
        r = ((long)x-screen.clipMidWidth())*((long)x-screen.clipMidWidth())+((long)y-screen.clipMidHeight())*((long)y-screen.clipMidHeight());
       } while (r > (screen.clipHeight()/2)*(screen.clipHeight()/2));

      color_t c = screen.black;
//      coord_t rc = 1;

      screen.polarToRect(sec_theta, screen.clipHeight()/2, xLine, yLine);
      dist = onLine(xCenter, yCenter, xLine, yLine, x, y);
      if ( dist < 0.005) {
        c = screen.white;
//        rc = 1;
      }

      screen.polarToRect(min_theta, screen.clipHeight()/2, xLine, yLine);
      dist = onLine(xCenter, yCenter, xLine, yLine, x, y);
      if ( dist < 0.005) {
        c = screen.green;
//        rc = 6;
      }

      screen.polarToRect(hr_theta, screen.clipHeight()/4, xLine, yLine);
      dist = onLine(xCenter, yCenter, xLine, yLine, x, y);
      if ( dist < 0.03) {
        c = screen.red;
//        rc = 8;
      }

  //  screen.fillCircle(x,y,rc,c);
  //  screen.fillRect(x,y,rc,rc,c);
      screen.drawPixel(x,y,c);
  }
}
