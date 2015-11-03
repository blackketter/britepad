#include "FastLED.h"

#include "BritepadShared.h"
#include "ClockApp.h"
#include "Debug.h"

void ClockApp::run() {
  millis_t mt = clock.millis();

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
    char textTime[6];

    screen.setFont(Arial_72_Bold);

    screen.setTextColor(screen.luminance(currentColor) < 128 ? screen.brighten(currentColor, 128) : currentColor, bgColor());
    sprintf(textTime, "%d:%02d", clock.hourFormat12(), clock.minute());
    coord_t w = screen.measureTextWidth(textTime);
    coord_t h = screen.measureTextHeight(textTime);
    screen.setCursor(screen.clipMidWidth() - w/2,
                     screen.clipMidHeight() - h/2);
    if (w != lastWidth) {
      clearScreen();
      lastWidth = w;
    }
    screen.drawText(textTime);

//  screen.setFont(Arial_20);
//  sprintf(textTime, "%02d", clock.second());
//  screen.drawText(textTime);

    screen.setFont(Arial_16);
    screen.setCursor(screen.clipMidWidth()+w/2,screen.clipMidHeight() - h/2);
    screen.drawText(clock.isAM() ? "am" : "pm");

    screen.setCursor(screen.clipMidWidth()+w/2,screen.clipMidHeight() + h/2 - screen.measureTextHeight(textTime));
    screen.drawTextF("%02d", clock.second());

    char longDate[100];
    clock.longDate(longDate);

    screen.setFont(Arial_12);
    screen.setCursor(screen.clipMidWidth()-screen.measureTextWidth(longDate)/2,
    screen.clipBottom()-((screen.clipBottom()-(screen.clipMidHeight() + h/2))/2  + screen.measureTextHeight(longDate)/2));
    screen.drawText(longDate);
    currentColor++;
}
