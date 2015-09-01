#include "BritepadApp.h"

uint8_t BritepadApp_SCRATCH_PAD[BritepadApp_SCRATCH_PAD_SIZE];

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

void BritepadApp::updateStatusBar(bool redraw) {

  if (displaysStatusBar()) {

    // set the clipping to the status bar
	  screen.setClipRect(0, statusBarTop, screen.width(), statusBarTop+statusBarHeight);

    if (redraw) {
      screen.fillRect(0, statusBarTop, screen.width(), statusBarHeight, statusBarBGColor());
      // draw title
      screen.setTextSize(statusBarHeight/screen.fontHeight/2);
      screen.setTextColor(statusBarFGColor());
      screen.setCursor( (width() - screen.measureTextH(name())) / 2, statusBarTop + (statusBarHeight-screen.measureTextV(name())) / 2);
      screen.drawText(name());
    }

    // set the clipping to the app area
    screen.setClipRect(left(), top(), left()+width(), top()+height());

  } else {
    screen.setClipRect(0,0,screen.width(), screen.height());
  }
};



