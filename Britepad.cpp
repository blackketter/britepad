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

Britepad::Britepad() {

}

BritepadApp* Britepad::getApp(appid_t appID) {

  int index = 0;
  BritepadApp* nextapp = getApp(index++);
  while (nextapp) {

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

  // count the enabled apps
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
//  DEBUG_PARAM_LN("No random app avaialble for mode ", m);
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


  if (newApp == BritepadApp::STAY_IN_APP) {
    return;
  } else if (newApp == BritepadApp::DEFAULT_APP) {
    newApp = randomApp(MOUSE);
    asMode = MOUSE;
  } else if (newApp == BritepadApp::SCREENSAVER_APP) {
    newApp = randomApp(SCREENSAVER);
    asMode = SCREENSAVER;
  } else if (newApp == BritepadApp::BACK_APP) {
    newApp = getApp(LauncherApp::ID);
    asMode = INTERACTIVE;
  }


  if (newApp == currApp) {
    if (currApp->getAppMode() != asMode) {
      DEBUG_PARAM_LN("begin asmode", asMode);
      DEBUG_PARAM_LN("curr app mode", currApp->getAppMode());

      currApp->begin(asMode);
    }
    return;
  }

  if (currApp)
    currApp->end(newApp);

  currApp = newApp;

  if (currApp) {
    currApp->drawBars();
    currApp->begin(asMode);
    if (asMode == SCREENSAVER) {
      screensaverStartedTime = pad.time();
    }
  }
}


void backlightCallback(void* data) {
  static uint8_t lastBacklight = 255;
  // if we detect proximity or touch, it's probably casting a shadow and we don't want to update
  millis_t lastTouch = min(pad.lastTouchedTime(ANY_PAD), pad.lastTouchedTime(PROXIMITY_SENSOR));
  if (pad.time() - lastTouch > PROXIMITY_DEAD_TIME) {
    // any ambient light greater than 255 is full brightness, 1 is the minimums
    uint8_t light = max(1,min( pad.getAmbientLight(), 255));

    // only ramp up/down one tick at a time
    if (lastBacklight < light) {
      lastBacklight++;
    } else if (lastBacklight > light) {
      lastBacklight--;
    }

    screen.backlight(lastBacklight);
  }
}

void statusBarCallback(void* data) {
    ((Britepad*)data)->updateStatusBar();
}

void Britepad::updateStatusBar() {
  // updates the clock in the status bar
  currApp->drawStatusBar(true);
}

void Britepad::begin() {

  // the launcher owns the apps and has created a splash app
  setApp(getApp(SplashApp::ID), SCREENSAVER);

// show the apps that have been loaded
  DEBUG_PARAM_LN("app count", appsAdded());
  int count=0;
  BritepadApp* anApp = getApp(count++);
  while (anApp) {
//    DEBUG_LN(count);
    DEBUG_LN(anApp->name());
//    DEBUG_LN((unsigned long)anApp);
    anApp = getApp(count++);
  }
  screen.fillScreen(screen.black);
  screen.backlight(screen.maxbrightness);
  backlightTimer.setMillis(ambientUpdateInterval, (timerCallback_t)backlightCallback, (void*)this, true);
  statusBarUpdateTimer.setMillis(1000, (timerCallback_t)statusBarCallback, (void*)this, true);
}

void Britepad::idle() {

  pad.update();

  if (pad.down(TOP_PAD)) {

    BritepadApp* nextApp = currApp->exitsTo();

    if (nextApp == BritepadApp::BACK_APP) {
      setNextApp(getApp(LauncherApp::ID));
      sound.swipe(DIRECTION_DOWN);
    } else if (nextApp == BritepadApp::DEFAULT_APP) {
      BritepadApp* wants = wantsToBeScreensaver();
      if (wants) {
        setNextApp(wants, SCREENSAVER);
      } else {
        setNextApp(randomApp(MOUSE), MOUSE);
      }
      sound.swipe(DIRECTION_UP);
    } else {
      setNextApp(nextApp);
    }

  } else if (currApp->isAppMode(SCREENSAVER) && (pad.down(SCREEN_PAD) || (pad.down(ANY_PAD) && !currApp->canBeInteractive()))) {
    // waking goes back to the mouse in the case that the user touched the screen (or any touch pad if it's not interactive)
    if (currApp->canBeMouse()) {
      currApp->begin(MOUSE);
    } else {
      setNextApp(randomApp(MOUSE), MOUSE);
    }

  } else if (pad.up(PROXIMITY_SENSOR) &&
             getApp(ClockApp::ID) &&  (getApp(ClockApp::ID))->getEnabled() && !currApp->isID(ClockApp::ID) &&
             currApp->isAppMode(SCREENSAVER) &&
             (pad.time() - pad.lastTouchedTime(ANY_PAD) > screensaverDelay))
  {
    setNextApp(getApp(ClockApp::ID), SCREENSAVER);
    DEBUG_LN("Proximity detected: showing clock");

  } else if (!currApp->disablesScreensavers()) {

   // check more often if somebody wants to be screensaver
   if (currApp->isAppMode(SCREENSAVER) &&
       (lastCheckWantsToBeScreensaver/checkWantsToBeScreensaverInterval != pad.time()/checkWantsToBeScreensaverInterval) &&
       !currApp->wantsToBeScreensaver() &&
       wantsToBeScreensaver()) {

        setNextApp(randomApp(SCREENSAVER), SCREENSAVER);

      lastCheckWantsToBeScreensaver = pad.time();
   }

    // let's check to see if we should run a screensaver
   if (currApp->isAppMode(SCREENSAVER) && (pad.time() - screensaverStartedTime) > screensaverSwitchInterval) {
      setNextApp(randomApp(SCREENSAVER), SCREENSAVER);

    // is it time for the screensaver to kick in?
    } else if (!currApp->isAppMode(SCREENSAVER) && (pad.time() - pad.lastTouchedTime(ANY_PAD) > screensaverDelay)) {
      setNextApp(randomApp(SCREENSAVER), SCREENSAVER);
    }
  }

  setApp(getNextApp(), getNextAppMode());

  setNextApp(BritepadApp::STAY_IN_APP);

  if (currApp) {
    currApp->run();
  } else {
    DEBUG_LN("currApp nil!");
  }

  // make sure the Timers get a chance to call their callbacks
  Timer::idle();
}
