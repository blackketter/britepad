#include "App.h"

#include "Dictionary.h"
extern EEPROMDictionary prefs;

#include "BritepadShared.h"

App* App::_appList = nullptr;

bool App::isCurrentApp() {
  return launcher.currentApp() == this;
};

void App::launchApp(App* app, AppMode mode) {
  launcher.launchApp(app, mode);
};

void App::setPrefs() {
  if (hasPrefs()) {
    uint8_t pref = (uint8_t)_enabled;
    prefs.set(id(), sizeof(pref), (uint8_t*)&pref);
  }
};

void App::getPrefs() {
  if (hasPrefs()) {
    uint8_t pref = (uint8_t)defaultEnabled();
    prefs.get(id(),  sizeof(pref), (uint8_t*)&pref);
    _enabled = (AppMode)pref;
  }
};

bool App::canBeAppMode(AppMode b) {
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

void App::switchAppMode(AppMode asMode) {
  _currAppMode = asMode;
}

void App::end() {
  _currAppMode = INACTIVE_MODE;
}

App* App::getAppByID(appid_t appID) {

  App* nextapp = getFirstApp();
  while (nextapp) {

    if (nextapp->isID(appID)) {
      return nextapp;
    }
    nextapp = nextapp->getNextApp();
  }
  return nullptr;
}


bool App::isID(appid_t match) {
  return !strcmp(match, id());
}

void App::sortApps() {
  App* oldList = getFirstApp();
  setFirstApp(nullptr);

  while (oldList) {
    App* i = oldList;
    App* highest = i;
    App* highestPrev = nullptr;

    App* p = nullptr;
    while (i) {
      if (strcasecmp(i->name(), highest->name()) > 0) {
        highestPrev = p;
        highest = i;
      }
      p = i;
      i = i->getNextApp();
    }

    // remove from the list
    App* n = highest->getNextApp();
    if (highestPrev) { highestPrev->setNextApp(n); } else { oldList = n; }

    App::addApp(highest);
  }
}

bool App::event(KeyEvent* key) {
  return false;
}


