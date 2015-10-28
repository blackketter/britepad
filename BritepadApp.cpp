#include "BritepadApp.h"
#include "AlarmApp.h"
#include "Icon.h"
#include "font_Arial.h"

BritepadApp* BritepadApp::STAY_IN_APP = (BritepadApp*)0;
BritepadApp* BritepadApp::DEFAULT_APP = (BritepadApp*)1;
BritepadApp* BritepadApp::BACK_APP = (BritepadApp*)2;
BritepadApp* BritepadApp::SCREENSAVER_APP = (BritepadApp*)3;

BritepadApp::BritepadApp()  {
  britepad.addApp(this);
}

void BritepadApp::resetClipRect() {
  coord_t top = displaysStatusBar() ? statusBarHeight : 0;
  coord_t bottom = displaysInfoBar() ? screen.height()-statusBarHeight : screen.height();
  screen.setClipRect(0, top, screen.width(), bottom);
}

const uint8_t bellIcon[] = {
  8,8,
  0b00011000,
  0b00111100,
  0b01100110,
  0b01100110,
  0b01100110,
  0b11111111,
  0b11111111,
  0b00011000
};

void BritepadApp::drawStatusBar(bool update) {
  if (displaysStatusBar()) {
    screen.setClipRect(0, 0, screen.width(), statusBarHeight);

    screen.setFont(Arial_8_Bold);
    screen.setTextColor(statusBarFGColor());

    if (!update) {
      screen.fillScreen(statusBarBGColor());

      // draw title
      const char* title = statusBarTitle();
      screen.setCursor( (screen.clipWidth() - screen.measureTextWidth(title)) / 2,
                         (statusBarHeight-screen.measureTextHeight(title)) / 2);
      screen.drawText(title);
    }

    // only include the clock if the app doesn't already draw a clock
    if (!displaysClock()) {
      // draw title
      screen.setFont(Arial_8_Bold);
      color_t textColor = screen.mix(statusBarFGColor(), statusBarBGColor());
      screen.setTextColor(textColor, statusBarBGColor());
      char shortTime[20];
      clock.shortTime(shortTime);
      char shortTimeSpaced[100];
      sprintf(shortTimeSpaced,"  %s ", shortTime);
      screen.setCursor( (screen.clipRight() - screen.measureTextWidth(shortTimeSpaced) - 2),
                        (statusBarHeight-screen.measureTextHeight(shortTimeSpaced)) / 2);
      screen.drawText(shortTimeSpaced);

      AlarmApp* alarm = (AlarmApp*)britepad.getApp(AlarmApp::ID);
      if (alarm && alarm->getAlarmEnabled()) {
        Icon(bellIcon).draw( screen.clipRight() - screen.measureTextWidth(shortTimeSpaced) - 10, 4, textColor) ;
      }

      clock.shortDate(shortTime);
      sprintf(shortTimeSpaced,"  %s ", shortTime);
      screen.setCursor( screen.clipLeft() + 2,
                        (statusBarHeight-screen.measureTextHeight(shortTimeSpaced)) / 2);
      screen.drawText(shortTimeSpaced);
    }

  }
  resetClipRect();
}

void BritepadApp::drawInfoBar(bool update) {
  if (displaysInfoBar()) {
    coord_t top = screen.height()-statusBarHeight;
    screen.setClipRect(0, top, screen.width(), screen.height());

    screen.setFont(Arial_8_Bold);
    screen.setTextColor(infoBarFGColor());

    const char* text = infoBarText();
    if (!update && text) {
      screen.fillScreen(infoBarBGColor());

      screen.setCursor( (screen.clipWidth() - screen.measureTextWidth(text)) / 2,
                         top + (statusBarHeight-screen.measureTextHeight(text)) / 2);
      screen.drawText(text);
    }

  }
  resetClipRect();
}

void BritepadApp::drawBars(bool update) {
  drawInfoBar(update);
  drawStatusBar(update);
}

bool BritepadApp::isID(appid_t match) {
  return !strcmp(match, id());
}

bool BritepadApp::canBeAppMode(AppMode b) {
  switch (b) {
    case SCREENSAVER:
      return canBeScreensaver();
    case MOUSE:
      return canBeMouse();
    case INTERACTIVE:
      return canBeInteractive();
    case INACTIVE:
      return true;
    default:
      return false;
  }
}

void BritepadApp::begin(AppMode asMode) {

  if (currAppMode != asMode) {
    if (currAppMode == MOUSE) {
      mouse.end();
    }

    if (asMode == MOUSE) {
      mouse.begin();
    }
  }
  currAppMode = asMode;
  screen.fillScreen(bgColor());

};  // initialize app state and draw first screen

void BritepadApp::end(BritepadApp* nextApp) {
  currAppMode = INACTIVE;
}
