#include "BritepadShared.h"

#include "Britepad.h"
#include "Types.h"
#include "Timer.h"

#include "ScreensaverApp.h"
#include "LauncherApp.h"
#include "MouseApp.h"

#include "Debug.h"

Britepad::Britepad(void) {

}

ScreensaverApp* Britepad::randomScreensaver(void) {

  // if an app wants to be a screensaver, then give it a chance
  ScreensaverApp* wants = wantsToBeScreensaver();
  if (wants) {
    return wants;
  }

  // count the enabled screensavers
  int count = 0;
  int index = 0;
  BritepadApp* nextapp = getApp(index++);
  while (nextapp) {
    if (nextapp->isScreensaver() && (((ScreensaverApp*)nextapp)->screensaverIsEnabled())) {
      count++;
    }
    nextapp = getApp(index++);
  }

  // pick a random one
  count = random(count);

  index = 0;
  nextapp = getApp(index++);
  while (nextapp) {
    if (nextapp->isScreensaver() && (((ScreensaverApp*)nextapp)->screensaverIsEnabled())) {
      if (count == 0) {
        return (ScreensaverApp*)nextapp;
      } else {
        count--;
      }
    }
    nextapp = getApp(index++);
  }
  return nil;
}

ScreensaverApp* Britepad::wantsToBeScreensaver() {
  int count = 0;
  BritepadApp* nextapp = getApp(count++);
  while (nextapp) {
    if (nextapp->wantsToBeScreensaver())
      return (ScreensaverApp*)nextapp;
    nextapp = getApp(count++);
  }
  return nil;
}

void Britepad::addApp(BritepadApp* app) {
  if (appCount < maxApps) {
    apps[appCount++] = app;
  } else {
    DEBUG_LN("TOO MANY APPS!");
  }
}

BritepadApp* Britepad::getApp(int appIndex) {
  return (appIndex >= appCount) ? nil : apps[appIndex];
}


void Britepad::setApp(BritepadApp* newApp) {

  if (newApp == 0) {
    DEBUG_LN("Set currApp to NIL!");
    return;
  }

  if (newApp == currApp) {
    return;
  }

  if (newApp == BritepadApp::DEFAULT_APP) {
    newApp = defaultApp;
  } else if (newApp == BritepadApp::BACK_APP) {
    newApp = launcherApp;
  }

  if (currApp)
    currApp->end(newApp);

  currApp = newApp;

  if (currApp) {
    currApp->updateStatusBarBounds();
    currApp->drawStatusBar();
    currApp->begin();

    if (currApp->isScreensaver()) {
      screensaverStartedTime = pad.time();
    }
  }
}



void Britepad::begin(void) {

// these are special apps that are shared across the enviroment
  launcherApp = new LauncherApp;
  defaultApp = new MouseApp;

  // the launcher owns the apps and has created a splash app
  setApp(launcherApp->getSplashApp());

// show the apps that have been loaded
  DEBUG_PARAM_LN("app count", appsAdded());
  int count=0;
  BritepadApp* anApp = getApp(count++);
  while (anApp) {
    DEBUG_LN(anApp->name());
    anApp = getApp(count++);
  }

}

void Britepad::idle(void) {

  pad.update();

  if (pad.down(TOP_PAD)) {
    // start or exit launcher
    if (currApp == launcherApp) {
      switchApp = defaultApp;
      sound.swipe(DIRECTION_UP);
    } else {
      switchApp = launcherApp;
      sound.swipe(DIRECTION_DOWN);
    }
  } else if (pad.touched(ANY_PAD) && currApp->isScreensaver()) {
    // touching resets screensaver
    switchApp = defaultApp;
  } else if (switchApp == BritepadApp::SCREENSAVER_APP) {
    switchApp = randomScreensaver();
  } else if (currApp->isScreensaver() && (pad.time() - screensaverStartedTime) > screensaverSwitchInterval) {
    switchApp = randomScreensaver();
  } else if (!currApp->isScreensaver() && (pad.time() - pad.lastTouchedTime(ANY_PAD) > screensaverDelay)) {
    switchApp = randomScreensaver();
  }

  setApp(switchApp);

  if (currApp) {
    switchApp = currApp->run();
  } else {
    DEBUG_LN("currApp nil!");
  }

  // make sure the Timers get a chance to call their callbacks
  Timer::idle();

}
