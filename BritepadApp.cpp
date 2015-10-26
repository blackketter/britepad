#include "BritepadApp.h"
#include "font_Arial.h"

BritepadApp* BritepadApp::STAY_IN_APP = (BritepadApp*)0;
BritepadApp* BritepadApp::DEFAULT_APP = (BritepadApp*)1;
BritepadApp* BritepadApp::BACK_APP = (BritepadApp*)2;
BritepadApp* BritepadApp::SCREENSAVER_APP = (BritepadApp*)3;

BritepadApp::BritepadApp(void)  {
  britepad.addApp(this);
}

void BritepadApp::resetClipRect(void) {
  coord_t top = displaysStatusBar() ? statusBarHeight : 0;
  coord_t bottom = displaysInfoBar() ? screen.height()-statusBarHeight : screen.height();
  screen.setClipRect(0, top, screen.width(), bottom);
}

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
      screen.setTextColor(screen.mix(statusBarFGColor(), statusBarBGColor()), statusBarBGColor());
      char shortTime[20];
      clock.shortTime(shortTime);
      char shortTimeSpaced[100];
      sprintf(shortTimeSpaced,"  %s ", shortTime);
      screen.setCursor( (screen.clipRight() - screen.measureTextWidth(shortTimeSpaced) - 2),
                        (statusBarHeight-screen.measureTextHeight(shortTimeSpaced)) / 2);
      screen.drawText(shortTimeSpaced);

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
