#include "BritepadApp.h"
#include "font_Arial.h"

uint8_t BritepadAppScratchPad[BritepadAppScratchPadSize];

BritepadApp* BritepadApp::STAY_IN_APP = (BritepadApp*)0;
BritepadApp* BritepadApp::DEFAULT_APP = (BritepadApp*)1;
BritepadApp* BritepadApp::BACK_APP = (BritepadApp*)2;
BritepadApp* BritepadApp::SCREENSAVER_APP = (BritepadApp*)3;

BritepadApp::BritepadApp(void)  {
  britepad.addApp(this);
}

void BritepadApp::drawStatusBar(bool update) {

  if (displaysStatusBar()) {
    // set the clipping to the status bar
	  screen.setClipRect(0, statusBarTop, screen.width(), statusBarTop+statusBarHeight);
    screen.setFont(Arial_8_Bold);
    screen.setTextColor(statusBarFGColor());

    if (!update) {
      screen.fillRect(0, statusBarTop, screen.clipWidth(), statusBarHeight, statusBarBGColor());
      // draw title
      const char* title = statusBarTitle();
      screen.setCursor( (screen.clipWidth() - screen.measureTextH(title)) / 2,
                         statusBarTop + (statusBarHeight-screen.measureTextV(title)) / 2);
      screen.drawText(title);
    }

    if (!displaysClock()) {
      // draw title
      screen.setFont(Arial_8_Bold);
      screen.setTextColor(screen.mix(statusBarFGColor(), statusBarBGColor()), statusBarBGColor());
      char shortTime[20];
      clock.shortTime(shortTime);
      char shortTimeSpaced[22];
      sprintf(shortTimeSpaced," %s ", shortTime);
      screen.setCursor( (screen.clipWidth() - screen.measureTextH(shortTimeSpaced) - 2),
                         statusBarTop + (statusBarHeight-screen.measureTextV(shortTimeSpaced)) / 2);
      screen.drawText(shortTimeSpaced);
    }

    // set the clipping to the app area
    screen.setClipRect(0, statusBarHeight, screen.width(), screen.height());
  } else {
    screen.setClipRect();
  }

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

  setAppMode(asMode);

  screen.fillScreen(bgColor());

};  // initialize app state and draw first screen

void BritepadApp::end(BritepadApp* nextApp) {
  setAppMode(INACTIVE);
}

void BritepadApp::setAppMode(AppMode newMode) {

  if (!canBeAppMode(newMode)) {
    DEBUG_PARAM_LN("cannot set new app mode", newMode);
    return;
  }

  if (currAppMode != newMode) {
    if (currAppMode == MOUSE) {
      mouse.end();
    }

    if (newMode == MOUSE) {
      mouse.begin();
    }
  }
  currAppMode = newMode;
}
