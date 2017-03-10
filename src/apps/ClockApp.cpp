#include "BritepadShared.h"
#include "ClockApp.h"

ClockApp theClockApp;

void ClockApp::run() {
  millis_t mt = Uptime::millis();

  if (lastUpdate == 0 || mt - lastUpdate > updateInterval()) {
    update();
    lastUpdate = mt;
  }
}

void ClockApp::begin(AppMode asMode) {
  ScreensaverApp::begin(asMode);
  lastUpdate = 0;
}

void ClockApp::update() {
    char textTime[6];  // "12:34\0"

    currentColor++;

    Time thisTime;

    thisTime.setSeconds(getClock().getSeconds());
    screen.setFont(Arial_72_Digits_Bold);

    screen.setTextColor(screen.luminance(currentColor) < 128 ? screen.brighten(currentColor, 128) : currentColor, bgColor());
    sprintf(textTime, "%d:%02d", thisTime.hourFormat12(), thisTime.minute());
    coord_t w = screen.measureTextWidth(textTime);
    coord_t h = screen.measureTextHeight(textTime);
    coord_t x = screen.clipMidWidth() - w/2;
    coord_t y = screen.clipMidHeight() - h/2;
    screen.setCursor(x,y);

    if (w != lastWidth) {
      clearScreen();
      lastWidth = w;
    }
    rect_t clip = { x,y,(coord_t)screen.measureTextWidth(textTime),(coord_t)(screen.measureTextHeight(textTime)+2)};
    screen.pushClipRect(&clip);
    screen.drawText(textTime);
    screen.pushClipRect(&clip);

    screen.setFont(Arial_16);

    screen.setCursor(screen.clipMidWidth()+w/2,screen.clipMidHeight() + h/2 - screen.measureTextHeight(textTime));
    screen.drawText(thisTime.isAM() ? "am" : "pm");

    screen.setCursor(screen.clipMidWidth()+w/2,screen.clipMidHeight() - h/2);
    screen.drawTextF("%02d", thisTime.second());

    char longDate[100];
    thisTime.longDate(longDate);

    screen.setFont(Arial_12);
    screen.setCursor(
      screen.clipMidWidth()-screen.measureTextWidth(longDate)/2,
      screen.clipBottom()-((screen.clipBottom()-(screen.clipMidHeight() + h/2))/2  + screen.measureTextHeight(longDate)/2));
    screen.drawText(longDate);
}
