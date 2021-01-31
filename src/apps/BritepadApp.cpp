#include "BritepadApp.h"
#include "AlarmApp.h"
#include "widgets/Icon.h"
#include "widgets/AppButton.h"

void BritepadApp::setClipRect(coord_t x, coord_t y, coord_t w, coord_t h) {
  screen.setClipRect(x,y,w,h);
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
    launcher.idle();
  } while (!done);
}

// initialize app state and clear screen
void BritepadApp::begin(AppMode asMode) {
  AppMode wasMode = _currAppMode;
  App::begin(asMode);
  if (asMode != INVISIBLE_MODE && wasMode != INVISIBLE_MODE) {
    // redraw, but only if we are not going to or coming from an invisible mode
    launcher.drawBars();
    clearScreen();
  }
  usbMouse.setBounds(screen.clipLeft(),screen.clipTop(), screen.clipWidth(), screen.clipHeight());
};

void BritepadApp::switchAppMode(AppMode asMode) {

  if (_currAppMode != asMode) {
    if (_currAppMode == MOUSE_MODE) {
      mousePad.end();
    }

    if (asMode == MOUSE_MODE) {
      mousePad.begin();
    }
    if (asMode == SCREENSAVER_MODE) {
      launcher.resetScreensaver();
    }
  }
  App::switchAppMode(asMode);
}

KeyEvent* BritepadApp::getNextEvent() {
  KeyEvent* e = keyEvents->getNextEvent();
  if (e) {
    launcher.resetScreensaver();
  };
  return e;
}

KeyEvent* BritepadApp::peekNextEvent() {
  KeyEvent* e = keyEvents->peekNextEvent();
  if (e) {
    launcher.resetScreensaver();
  };
  return e;
}

AppButton* BritepadApp::newAppButton() {
  return new AppButton(this);
}


void BritepadApp::delay(millis_t d) {
  // do calculations in micros_t for more accuracy
  micros_t end = Uptime::micros() + d*1000;
  do {
    launcher.idle();
    yield();
  } while (Uptime::micros() <= end);
}

bool BritepadApp::timeToLeave() {
  return ((Uptime::millis() -  pad.time()) > _maxRunTime);
}

