#include "App.h"

App* App::_appList = nullptr;

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

App* App::findApp(const char* s) {
  App* a = getAppByID(s);
  if (a == nullptr) {
    int n = atoi(s);
    if (n) {
      a = App::getFirstApp();
      while (n > 1) {
        a = a->getNextApp();
        n--;
      }
    }
  }
  return a;
}

bool App::event(KeyEvent* key) {
  return false;
}


