#include "FastLED.h"

#include "BritepadShared.h"
#include "ClockApp.h"
#include "Debug.h"

BritepadApp* ClockApp::run(void) {
  millis_t mt = clock.millis();

  if (mt - lastRedraw > redrawInterval) {
    redraw();
    lastRedraw = mt;
  }
  return STAY_IN_APP;
}

void ClockApp::redraw(void) {
    char textTime[6];

    screen.setFont(Arial_72);

    screen.setTextColor(screen.luminance(currentColor) < 128 ? screen.brighten(currentColor, 128) : currentColor, bgColor());
    currentColor++;
    sprintf(textTime, "%d:%02d", clock.hourFormat12(), clock.minute());
    coord_t w = screen.measureTextH(textTime);
    coord_t h = screen.measureTextV(textTime);
    screen.setCursor(screen.clipWidth()/2 - w/2,
                     screen.clipHeight()/2 - h/2);
    if (w != lastWidth) {
      clearScreen();
      lastWidth = w;
    }
    screen.drawText(textTime);

//  screen.setFont(Arial_20);
//  sprintf(textTime, "%02d", clock.second());
//  screen.drawText(textTime);

    screen.setFont(Arial_16);
    screen.setCursor(screen.clipWidth()/2+w/2,screen.clipHeight()/2 + h/2 - screen.measureTextV(textTime));
    screen.drawText(clock.isAM() ? "am" : "pm");
}
/*
// Draws analog clock
  int x = random(screen.clipWidth());
  int y = random(screen.clipHeight());
  long r = random(40);
  time_t tt = now();
  long h = hourFormat12(tt);
  long m = minute(tt);
  long s = second(tt);
  long t = h*60+m;

  // clip out the area where we'll draw the text
  screen.setClipRect(0, 20, screen.clipWidth(), screen.clipHeight());

  screen.fillCircle(x, y, r, currentColor);
  screen.drawCircle(x, y, r, ~currentColor);
  // hands are 80% the radius of the clock
  r = (r * 8)/10;

  screen.drawLine(x,y, x + r*((long)sin8((t*255)/720)-128)/128/2, y - r*((long)cos8((t*255)/720)-128)/128/2, ~currentColor);
  screen.drawLine(x,y, x + r*((long)sin8((m*255)/60)-128)/128, y - r*((long)cos8((m*255)/60)-128)/128, ~currentColor);

  screen.setClipRect();  // reset the clip rect
  screen.setCursor(0,0);
  screen.setTextColor(currentColor, screen.black);

  if (h == 0) h=12;
  screen.drawTextF("%2d:%02d:%02d", h, m, s);
  currentColor++;
*/

