#include "BritepadShared.h"

#include "Britepad.h"
#include "Types.h"
#include "Timer.h"

#include "LauncherApp.h"
#include "ClockApp.h"
#include "SplashApp.h"

#define DEBUG_ON 1
#include "Debug.h"

#define PROXIMITY_DEAD_TIME (1000)

Britepad::Britepad(void) {

}

BritepadApp* Britepad::getApp(appid_t appID) {

  int index = 0;
  BritepadApp* nextapp = getApp(index++);
  while (nextapp) {
//    DEBUG_PARAM_LN("checking", (unsigned long)nextapp);
//    DEBUG_LN(nextapp->id());

    if (nextapp->isID(appID)) {
      return nextapp;
    } else {
      nextapp = getApp(index++);
    }
  }
  return nil;
}

BritepadApp* Britepad::randomApp(AppMode m) {

  if (m == SCREENSAVER) {
    // if an app wants to be a screensaver, then give it a chance
    BritepadApp* wants = wantsToBeScreensaver();
    if (wants) {
      return wants;
    }
  }

  // count the enabled screensavers
  int count = 0;
  int index = 0;
  BritepadApp* nextapp = getApp(index++);
  while (nextapp) {
    if (nextapp->canBeAppMode(m) && ((nextapp)->getEnabled())) {
      count++;
    }
    nextapp = getApp(index++);
  }

  // pick a random one
  count = random(count);

  index = 0;
  nextapp = getApp(index++);
  while (nextapp) {
    if (nextapp->canBeAppMode(m) && ((nextapp)->getEnabled())) {
      if (count == 0) {
        return nextapp;
      } else {
        count--;
      }
    }
    nextapp = getApp(index++);
  }
  DEBUG_PARAM_LN("No random app avaialble for mode ", m);
  return nil;
}

BritepadApp* Britepad::wantsToBeScreensaver() {
  int count = 0;
  BritepadApp* nextapp = getApp(count++);
  while (nextapp) {
    if (nextapp->wantsToBeScreensaver())
      return nextapp;
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


void Britepad::setApp(BritepadApp* newApp, AppMode asMode) {


  if (newApp == 0) {
    // just STAY_IN_APP
    return;
  } else if (newApp == BritepadApp::DEFAULT_APP) {
    newApp = randomApp(MOUSE);
    asMode = MOUSE;
  } else if (newApp == BritepadApp::BACK_APP) {
    newApp = getApp(LauncherApp::ID);
    asMode = INTERACTIVE;
  }

  if (asMode == SCREENSAVER) {
    screensaverStartedTime = pad.time();
  }

  if (newApp == currApp) {
    currApp->setAppMode(asMode);
    return;
  }

  if (currApp)
    currApp->end(newApp);

  currApp = newApp;

  if (currApp) {
    currApp->drawStatusBar();
    currApp->begin(asMode);
  }
}


void backlightCallback(void* data) {
  // if we detect proximity, it's probably casting a shadow and we don't want to update
  if (pad.time() - pad.lastTouchedTime(PROXIMITY_SENSOR) > PROXIMITY_DEAD_TIME) {
    // any ambient light greater than 255 is full brightness, 1 is the minimums
    uint8_t light = max(1,min( pad.getAmbientLight(), 255));
    screen.backlight(light);
  }
}

void statusBarCallback(void* data) {
    ((Britepad*)data)->updateStatusBar();
}

void Britepad::updateStatusBar(void) {
  currApp->drawStatusBar(true);
}

void Britepad::begin(void) {

  // the launcher owns the apps and has created a splash app
  setApp(getApp(SplashApp::ID), SCREENSAVER);

// show the apps that have been loaded
  DEBUG_PARAM_LN("app count", appsAdded());
  int count=0;
  BritepadApp* anApp = getApp(count++);
  while (anApp) {
    DEBUG_LN(anApp->name());
    anApp = getApp(count++);
  }
  screen.fillScreen(screen.black);
  screen.backlight(screen.maxbrightness);
  backlightTimer.setMillis(ambientUpdateInterval, (timerCallback_t)backlightCallback, (void*)this, true);
  statusBarUpdateTimer.setMillis(1000, (timerCallback_t)statusBarCallback, (void*)this, true);
}

void Britepad::idle(void) {

  AppMode asMode = INTERACTIVE;

  pad.update();

  if (pad.down(TOP_PAD)) {

    if (!currApp->isID(LauncherApp::ID)) {
      switchApp = getApp(LauncherApp::ID);
      sound.swipe(DIRECTION_DOWN);
    } else {
      switchApp = randomApp(MOUSE);
      asMode = MOUSE;
      sound.swipe(DIRECTION_UP);
    }

  } else if (currApp->isAppMode(SCREENSAVER) && (pad.down(SCREEN_PAD) || (pad.down(ANY_PAD) && !currApp->canBeInteractive()))) {
    // waking goes back to the mouse in the case that the user touched the screen (or any touch pad if it's not interactive)
    if (currApp->canBeMouse()) {
      currApp->setAppMode(MOUSE);
    } else {
      switchApp = randomApp(MOUSE);
    }
    asMode = MOUSE;

  } else if (getApp(ClockApp::ID) &&  (getApp(ClockApp::ID))->getEnabled() && currApp->isAppMode(SCREENSAVER) && !currApp->isID(ClockApp::ID) && pad.up(PROXIMITY_SENSOR)) {
    switchApp = getApp(ClockApp::ID);
    asMode = SCREENSAVER;
    DEBUG_LN("Proximity detected: showing clock");

  } else if (!currApp->disablesScreensavers()) {
    // let's check to see if we should run a screensaver

    // has the previous app asked to go right to screensaver?
    if (switchApp == BritepadApp::SCREENSAVER_APP) {
      asMode = SCREENSAVER;

    // is it time to switch to another screensaver?
    } else if (currApp->isAppMode(SCREENSAVER) && (pad.time() - screensaverStartedTime) > screensaverSwitchInterval) {
      asMode = SCREENSAVER;

    // is it time for the screensaver to kick in?
    } else if (!currApp->isAppMode(SCREENSAVER) && (pad.time() - pad.lastTouchedTime(ANY_PAD) > screensaverDelay)) {
      asMode = SCREENSAVER;
    }

    if (asMode == SCREENSAVER) {
      switchApp = randomApp(asMode);
    }

  }

  setApp(switchApp, asMode);

  if (currApp) {
    switchApp = currApp->run();
  } else {
    DEBUG_LN("currApp nil!");
  }

  // make sure the Timers get a chance to call their callbacks
  Timer::idle();
}
