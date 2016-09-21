#include "BritepadShared.h"
#include "BriteLiteApp.h"
#include "Debug.h"

BriteLiteApp theBriteLiteApp;

void BriteLiteApp::begin(AppMode asMode) {
  if (!dots) {
    dots = new HexDotMatrix(screen.clipLeft(), screen.clipTop(), screen.clipWidth(), screen.clipHeight(), getDotsWide(), getDotsHigh());
  }
  DotsDisplayApp::begin(asMode);
}

void BriteLiteApp::run() {
  ScreensaverApp::run();
  if (pad.touched(SCREEN_PAD)) {
    int x, y;
    if (dots->hit(pad.x(), pad.y(), &x, &y)) {
      dots->setDot(x,y, lastColor++ | 0x8000);
      dots->updateDot(x,y);
    }
  } else if (isAppMode(SCREENSAVER_MODE)) {
  switch (random(8)) {
    case 0:
      cursor_x++;
      cursor_y--;
      break;
    case 1:
      cursor_x++;
      break;
    case 2:
      cursor_x++;
      cursor_y++;
      break;
    case 3:
      cursor_y++;
      break;
    case 4:
    case 6:
    case 7:
      cursor_x--;
      break;
    case 5:
      cursor_y--;
      break;
  }
  cursor_y = min(dots->getDotsHigh()-1,max(0,cursor_y));
  cursor_x = min(dots->getDotsWide()-1,max(0,cursor_x));

  dots->setDot(cursor_x, cursor_y, lastColor++ | 0x8000);
  dots->updateDot(cursor_x, cursor_y);
  }
}


