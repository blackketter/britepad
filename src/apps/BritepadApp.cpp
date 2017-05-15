#include "BritepadApp.h"
#include "AlarmApp.h"
#include "widgets/Icon.h"
#include "AppButton.h"

BritepadApp* BritepadApp::STAY_IN_APP = (BritepadApp*)0;
BritepadApp* BritepadApp::BACK_APP = (BritepadApp*)2;
BritepadApp* BritepadApp::A_MOUSE_APP = (BritepadApp*)1;
BritepadApp* BritepadApp::A_SCREENSAVER_APP = (BritepadApp*)3;

BritepadApp::BritepadApp()  {
  Britepad::addApp(this);
}

void BritepadApp::resetClipRect() {
  coord_t top = displaysStatusBar() ? _statusBarHeight : 0;
  coord_t bottom = displaysInfoBar() ? screen.height()-_statusBarHeight : screen.height();
  screen.setClipRect(0, top, screen.width(), bottom-top);
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
    screen.setClipRect(0, 0, screen.width(), _statusBarHeight);

    screen.setFont(Arial_8_Bold);
    screen.setTextColor(statusBarFGColor());

    if (!update) {
      screen.fillScreen(statusBarBGColor());

      // draw title
      const char* title = statusBarTitle();
      screen.setCursor( (screen.clipWidth() - screen.measureTextWidth(title)) / 2,
                         (_statusBarHeight-screen.measureTextHeight(title)) / 2);
      screen.drawText(title);
    }

    // only include the clock if the app doesn't already draw a clock
    if (!displaysClock() && clock.hasBeenSet()) {
      // draw title
      screen.setFont(Arial_8_Bold);
      color_t textColor = screen.mix(statusBarFGColor(), statusBarBGColor());
      screen.setTextColor(textColor, statusBarBGColor());
      char shortTime[20];
      clock.shortTime(shortTime);
      char shortTimeSpaced[100];
      sprintf(shortTimeSpaced,"  %s ", shortTime);
      screen.setCursor( (screen.clipRight() - screen.measureTextWidth(shortTimeSpaced) - 2),
                        (_statusBarHeight-screen.measureTextHeight(shortTimeSpaced)) / 2);
      screen.drawText(shortTimeSpaced);

      AlarmApp* alarm = (AlarmApp*)getAppByID(AlarmApp::ID);
      if (alarm && alarm->getAlarmEnabled()) {
        Icon(bellIcon).draw( screen.clipRight() - screen.measureTextWidth(shortTimeSpaced) - 10, 4, textColor) ;
      }

      clock.shortDate(shortTime);
      sprintf(shortTimeSpaced,"  %s ", shortTime);
      screen.setCursor( screen.clipLeft() + 2,
                        (_statusBarHeight-screen.measureTextHeight(shortTimeSpaced)) / 2);
      screen.drawText(shortTimeSpaced);
    }

  }
  resetClipRect();
}

void BritepadApp::drawInfoBar(bool update) {
  if (displaysInfoBar()) {
    coord_t top = screen.height()-_statusBarHeight;
    screen.setClipRect(0, top, screen.width(), screen.height()-top);

    screen.setFont(Arial_8_Bold);
    screen.setTextColor(infoBarFGColor());

    const char* text = infoBarText();
    if (!update && text) {
      screen.fillScreen(infoBarBGColor());

      screen.setCursor( (screen.clipWidth() - screen.measureTextWidth(text)) / 2,
                         top + (_statusBarHeight-screen.measureTextHeight(text)) / 2);
      screen.drawText(text);
    }

  }
  resetClipRect();
}

void BritepadApp::clearScreen() {
  //idle because fillScreen is slow
  coord_t start = screen.clipTop();
  coord_t bottom = screen.clipBottom();
  coord_t left = screen.clipLeft();
  coord_t width = screen.clipWidth();
  coord_t height = screen.height() / 10;  // do it in 10 segments to give time to idle
  bool done = false;
  do {
    if ((height+start)>bottom) {
      height = bottom - start;
      done = true;
    }
    screen.fillRect(left,start,width,height, bgColor());
    start += height;
    britepad.idle();
  } while (!done);
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
    case SCREENSAVER_MODE:
      return canBeScreensaver();
    case MOUSE_MODE:
      return canBeMouse();
    case INTERACTIVE_MODE:
      return canBeInteractive();
    case INACTIVE_MODE:
      return true;
    default:
      return false;
  }
}

void BritepadApp::switchAppMode(AppMode asMode) {
  if (_currAppMode != asMode) {
    if (_currAppMode == MOUSE_MODE) {
      mouse.end();
    }

    if (asMode == MOUSE_MODE) {
      mouse.begin();
    }
    if (asMode == SCREENSAVER_MODE) {
      britepad.resetScreensaver();
    }
  }
  _currAppMode = asMode;
}

// initialize app state and clear screen
void BritepadApp::begin(AppMode asMode) {
  BritepadApp::switchAppMode(asMode);
  if (asMode != INVISIBLE_MODE) {
    drawBars();
    clearScreen();
  }
};

void BritepadApp::end() {
  _currAppMode = INACTIVE_MODE;
}

AppButton* BritepadApp::newAppButton() {
  return new AppButton(this);
}

