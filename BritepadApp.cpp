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

coord_t BritepadApp::top(void) {
  if (displaysStatusBar() && statusBarTop == 0) {
    return statusBarHeight;
  } else {
    return 0;
  };
};

coord_t BritepadApp::height(void) {
  coord_t h = screen.height();
  if (displaysStatusBar()) {
    h -= statusBarHeight;
  }
  return h;
};


coord_t BritepadApp::left(void) {
  return 0;
};


coord_t BritepadApp::width(void) {
  return screen.width();
};

void BritepadApp::drawStatusBar(void) {
  if (displaysStatusBar()) {
    // set the clipping to the status bar
	  screen.setClipRect(0, statusBarTop, screen.width(), statusBarTop+statusBarHeight);

    screen.fillRect(0, statusBarTop, screen.width(), statusBarHeight, statusBarBGColor());
    // draw title
    screen.setFont(Arial_8);
    screen.setTextSize(statusBarHeight/screen.fontHeight()/2);
    screen.setTextColor(statusBarFGColor());
    const char* title = statusBarTitle();
    screen.setCursor( (width() - screen.measureTextH(title)) / 2,
                       statusBarTop + (statusBarHeight-screen.measureTextV(title)) / 2);
    screen.drawText(title);

    updateStatusBarBounds();
  }
}

void BritepadApp::updateStatusBarBounds(void) {
  if (displaysStatusBar()) {
    // set the clipping to the app area
    screen.setClipRect(left(), top(), left()+width(), top()+height());

  } else {
    screen.setClipRect();
  }
};

bool BritepadApp::isID(appid_t match) {
  return !strcmp(match, id());
}

