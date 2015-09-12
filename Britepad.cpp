#include "BritepadShared.h"

#include "Britepad.h"
#include "Types.h"
#include "Timer.h"

#include "ScreensaverApp.h"
#include "LauncherApp.h"
#include "MouseApp.h"
#include "SplashApp.h"

#include "Debug.h"

Britepad::Britepad(void) {

}

BritepadApp* Britepad::getApp(appid_t appID) {
  DEBUG_LN("Looking for:");
  DEBUG_LN(appID);

  int index = 0;
  BritepadApp* nextapp = getApp(index++);
  while (nextapp) {
    DEBUG_PARAM_LN("checking", (unsigned long)nextapp);
    DEBUG_LN(nextapp->id());

    if (nextapp->isID(appID)) {
      return nextapp;
    } else {
      nextapp = getApp(index++);
    }
  }
  return nil;
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
    // just STAY_IN_APP
    return;
  }

  if (newApp == currApp) {
    return;
  }

  if (newApp == BritepadApp::DEFAULT_APP) {
    newApp = defaultApp;
  } else if (newApp == BritepadApp::BACK_APP) {
    newApp = getApp(LauncherApp::ID);
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


void backlightCallback(void* data) {
  // if we detect proximity, it's probably casting a shadow and we don't want to update
  if (pad.getProximity() < 100) {
    // any ambient light greater than 255 is full brightness, 1 is the minimums
    uint8_t light = max(1,min( pad.getAmbientLight(), 255));
    screen.backlight(light);
  }
}

void Britepad::begin(void) {

// these are special apps that are shared across the enviroment
  defaultApp = getApp(MouseApp::ID);

  // the launcher owns the apps and has created a splash app
  setApp(getApp(SplashApp::ID));

// show the apps that have been loaded
  DEBUG_PARAM_LN("app count", appsAdded());
  int count=0;
  BritepadApp* anApp = getApp(count++);
  while (anApp) {
    DEBUG_LN(anApp->name());
    anApp = getApp(count++);
  }

  backlightTimer.setMillis(ambientUpdateInterval, (timerCallback_t)backlightCallback, (void*)this, true);

}

void Britepad::idle(void) {

  pad.update();

  if (pad.down(TOP_PAD)) {
    // start or exit launcher
    if (currApp == getApp(LauncherApp::ID)) {
      switchApp = defaultApp;
      sound.swipe(DIRECTION_UP);
    } else {
      switchApp = getApp(LauncherApp::ID);
      sound.swipe(DIRECTION_DOWN);
    }
  } else if (pad.touched(ANY_PAD) && currApp->isScreensaver()) {
    // touching resets screensaver
    switchApp = defaultApp;
  } else if (!currApp->disablesScreensavers()) {
    // let's check for screensavers
    if (switchApp == BritepadApp::SCREENSAVER_APP) {
      switchApp = randomScreensaver();
    } else if (currApp->isScreensaver() && (pad.time() - screensaverStartedTime) > screensaverSwitchInterval) {
      switchApp = randomScreensaver();
    } else if (!currApp->isScreensaver() && (pad.time() - pad.lastTouchedTime(ANY_PAD) > screensaverDelay)) {
      switchApp = randomScreensaver();
    }
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
