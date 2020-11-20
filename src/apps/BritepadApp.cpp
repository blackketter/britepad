#include "BritepadApp.h"
#include "AlarmApp.h"
#include "widgets/Icon.h"
#include "widgets/AppButton.h"

BritepadApp* BritepadApp::_appList = nullptr;

void BritepadApp::setClipRect(coord_t x, coord_t y, coord_t w, coord_t h) {
  screen.setClipRect(x,y,w,h);
}

bool BritepadApp::isCurrentApp() {
  return britepad.currentApp() == this;
};

void BritepadApp::launchApp(BritepadApp* app, AppMode mode) {
  britepad.launchApp(app, mode);
};

KeyEvent* BritepadApp::getNextEvent() {
  KeyEvent* e = keyEvents.getNextEvent();
  if (e) {
    britepad.resetScreensaver();
  };
  return e;
}

KeyEvent* BritepadApp::peekNextEvent() {
  KeyEvent* e = keyEvents.peekNextEvent();
  if (e) {
    britepad.resetScreensaver();
  };
  return e;
}

bool BritepadApp::timeToLeave() {
  return ((Uptime::millis() -  pad.time()) > _maxRunTime);
}

void BritepadApp::setPrefs() {
  if (hasPrefs()) {
    uint8_t pref = (uint8_t)_enabled;
    prefs.set(id(), sizeof(pref), (uint8_t*)&pref);
  }
};

void BritepadApp::getPrefs() {
  if (hasPrefs()) {
    uint8_t pref = (uint8_t)defaultEnabled();
    prefs.get(id(),  sizeof(pref), (uint8_t*)&pref);
    _enabled = (AppMode)pref;
  }
};

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

void BritepadApp::delay(millis_t d) {
  // do calculations in micros_t for more accuracy
  micros_t end = Uptime::micros() + d*1000;
  do {
    britepad.idle();
    yield();
  } while (Uptime::micros() <= end);
}

bool BritepadApp::canBeAppMode(AppMode b) {
  switch (b) {
    case SCREENSAVER_MODE:
      return canBeScreensaver();
    case MOUSE_MODE:
      return canBeMouse();
    case INTERACTIVE_MODE:
      return canBeInteractive();
    case SETUP_MODE:
      return canBeSetup();
    case INACTIVE_MODE:
      return true;
    default:
      return false;
  }
}

void BritepadApp::switchAppMode(AppMode asMode) {

  if (_currAppMode != asMode) {
    if (_currAppMode == MOUSE_MODE) {
      mousePad.end();
    }

    if (asMode == MOUSE_MODE) {
      mousePad.begin();
    }
    if (asMode == SCREENSAVER_MODE) {
      britepad.resetScreensaver();
    }
  }
  _currAppMode = asMode;
}

// initialize app state and clear screen
void BritepadApp::begin(AppMode asMode) {
  AppMode wasMode = _currAppMode;
  BritepadApp::switchAppMode(asMode);
  if (asMode != INVISIBLE_MODE && wasMode != INVISIBLE_MODE) {
    // redraw, but only if we are not going to or coming from an invisible mode
    britepad.drawBars();
    clearScreen();
  }
  usbMouse.setBounds(screen.clipLeft(),screen.clipTop(), screen.clipWidth(), screen.clipHeight());
};

void BritepadApp::end() {
  _currAppMode = INACTIVE_MODE;
}

AppButton* BritepadApp::newAppButton() {
  return new AppButton(this);
}

BritepadApp* BritepadApp::getAppByID(appid_t appID) {

  BritepadApp* nextapp = getFirstApp();
  while (nextapp) {

    if (nextapp->isID(appID)) {
      return nextapp;
    }
    nextapp = nextapp->getNextApp();
  }
  return nullptr;
}


bool BritepadApp::isID(appid_t match) {
  return !strcmp(match, id());
}

void BritepadApp::sortApps() {
  BritepadApp* oldList = getFirstApp();
  setFirstApp(nullptr);

  while (oldList) {
    BritepadApp* i = oldList;
    BritepadApp* highest = i;
    BritepadApp* highestPrev = nullptr;

    BritepadApp* p = nullptr;
    while (i) {
      if (strcasecmp(i->name(), highest->name()) > 0) {
        highestPrev = p;
        highest = i;
      }
      p = i;
      i = i->getNextApp();
    }

    // remove from the list
    BritepadApp* n = highest->getNextApp();
    if (highestPrev) { highestPrev->setNextApp(n); } else { oldList = n; }

    BritepadApp::addApp(highest);
  }
}

bool BritepadApp::event(KeyEvent* key) {
  return false;
}


