#include <Time.h>
#include "FastLED.h"

#include "Britepad.h"
#include "ClockApp.h"
#include "Debug.h"

BritepadApp* ClockApp::run(void) {
  int x = random(screen.width());
  int y = random(screen.height());
  long r = random(40);
  time_t tt = now();
  long h = hourFormat12(tt);
  long m = minute(tt);
  long s = second(tt);
  long t = h*60+m;

  // clip out the area where we'll draw the text
  screen.setClipRect(0, 20, screen.width(), screen.height());

  screen.fillCircle(x, y, r, currentColor);
  screen.drawCircle(x, y, r, ~currentColor);
  // hands are 80% the radius of the clock
  r = (r * 8)/10;

  screen.drawLine(x,y, x + r*((long)sin8((t*255)/720)-128)/128/2, y - r*((long)cos8((t*255)/720)-128)/128/2, ~currentColor);
  screen.drawLine(x,y, x + r*((long)sin8((m*255)/60)-128)/128, y - r*((long)cos8((m*255)/60)-128)/128, ~currentColor);

  screen.setClipRect();  // reset the clip rect
  screen.setCursor(0,0);
  screen.setTextSize(2);
  screen.setTextColor(currentColor, screen.black);

  if (h == 0) h=12;
  screen.drawTextF("%2d:%02d:%02d", h, m, s);
  currentColor++;

  return nil;
}

