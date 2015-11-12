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

BritepadApp* Britepad::getAppByID(appid_t appID) {

  BritepadApp* nextapp = appList;
  while (nextapp) {

    if (nextapp->isID(appID)) {
      return nextapp;
    }
    nextapp = nextapp->getNextApp();
  }
  return nullptr;
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
  BritepadApp* nextapp = appList;
  while (nextapp) {
    if (nextapp->canBeAppMode(m) && ((nextapp)->getEnabled())) {
      count++;
    }
    nextapp = nextapp->getNextApp();
  }

  // pick a random one
  count = random(count);

  nextapp = appList;
  while (nextapp) {
    if (nextapp->canBeAppMode(m) && ((nextapp)->getEnabled())) {
      if (count == 0) {
        return nextapp;
      } else {
        count--;
      }
    }
    nextapp = nextapp->getNextApp();
  }
  DEBUG_PARAM_LN("No random app avaialble for mode ", m);
  return nullptr;
}

BritepadApp* Britepad::wantsToBeScreensaver() {

  BritepadApp* nextapp = appList;
  while (nextapp) {
    if (nextapp->wantsToBeScreensaver())
      return nextapp;
    nextapp = nextapp->getNextApp();
  }
  return nullptr;
}

void Britepad::addApp(BritepadApp* app) {
  app->setNextApp(appList);
  appList = app;
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
    newApp = getAppByID(LauncherApp::ID);
    asMode = INTERACTIVE;
  }

  launchedAppPtr = newApp;

  if (newApp == currApp) {
    if (currApp->getAppMode() != asMode) {
       currApp->setAppMode(asMode);
    }
    return;
  }

  if (currApp) {
//    DEBUG_PARAM_LN("Ending App", currApp->name());
    currApp->end();
  }

  currApp = newApp;

  if (currApp) {
//    DEBUG_PARAM_LN("Starting App", currApp->name());
    currApp->drawBars();
    currApp->begin();
    currApp->setAppMode(asMode);
    if (asMode == SCREENSAVER) {
      screensaverStartedTime = pad.time();
    }
  }
}


void backlightCallback(void* data) {
  uint8_t lastBacklight = screen.getBacklight();

  // if we detect proximity or touch, it's probably casting a shadow and we don't want to update
  millis_t lastTouch = max(pad.lastTouchedTime(ANY_PAD), pad.lastTouchedTime(PROXIMITY_SENSOR));
  if ((pad.time() - lastTouch) > PROXIMITY_DEAD_TIME) {
    // any ambient light greater than 255 is full brightness, 1 is the minimums
    uint8_t light = max(1,min( pad.getAmbientLight(), 255));

    int delta;
    const int downRate = 2;
    const int upRate = 4;
    // ramp down slow, ramp up fast
    if (lastBacklight < light) {
      if ((light - lastBacklight) > upRate) {
        delta = upRate;
      } else {
        delta = 1;
      }
      lastBacklight += delta;
    } else if (lastBacklight > light) {
      if ((lastBacklight - light) > downRate) {
        delta = downRate;
      } else {
        delta = 1;
      }
      lastBacklight -= delta;
    }

    screen.setBacklight(lastBacklight);
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

  // assumes that the splashapp has been created and added to list
  setApp(getAppByID(SplashApp::ID), SCREENSAVER);

// show the apps that have been loaded
  BritepadApp* anApp = appList;
  int count = 1;
  while (anApp != nullptr) {
//    DEBUG_LN(count);
//    DEBUG_LN(anApp->name());
//    DEBUG_LN((unsigned long)anApp);
    anApp = anApp->getNextApp();
    count++;
  }

  screen.fillScreen(screen.black);
  screen.setBacklight(screen.maxbrightness);
  backlightTimer.setMillis(ambientUpdateInterval, (timerCallback_t)backlightCallback, (void*)this, true);
  statusBarUpdateTimer.setMillis(1000, (timerCallback_t)statusBarCallback, (void*)this, true);
}

void Britepad::idle() {

  pad.update();

  if (pad.down(TOP_PAD)) {
    DEBUG_LN("Toppad down");
    if (currApp) {
      BritepadApp* nextApp = currApp->exitsTo();
      launchApp(nextApp);
    } else {
      DEBUG_LN("No currapp!");
    }
  } else if (currApp->isAppMode(SCREENSAVER) && (pad.down(SCREEN_PAD) || (pad.down(ANY_PAD) && !currApp->canBeInteractive()))) {
    // waking goes back to the mouse in the case that the user touched the screen (or any touch pad if it's not interactive)
    if (currApp->canBeMouse()) {
      currApp->setAppMode(MOUSE);
    } else {
      launchApp(BritepadApp::DEFAULT_APP, MOUSE);
    }

  } else if (pad.up(PROXIMITY_SENSOR) &&
             getAppByID(ClockApp::ID) &&  (getAppByID(ClockApp::ID))->getEnabled() && !currApp->isID(ClockApp::ID) &&
             currApp->isAppMode(SCREENSAVER) &&
             (pad.time() - pad.lastTouchedTime(ANY_PAD) > screensaverDelay))
  {
    launchApp(getAppByID(ClockApp::ID), SCREENSAVER);
    DEBUG_LN("Proximity detected: showing clock");

  } else if (!currApp->disablesScreensavers()) {

   // check more often if somebody wants to be screensaver
   if (currApp->isAppMode(SCREENSAVER) &&
       (lastCheckWantsToBeScreensaver/checkWantsToBeScreensaverInterval != pad.time()/checkWantsToBeScreensaverInterval) &&
       !currApp->wantsToBeScreensaver() &&
       wantsToBeScreensaver()) {

      launchApp(BritepadApp::SCREENSAVER_APP, SCREENSAVER);

      lastCheckWantsToBeScreensaver = pad.time();
   }

    // let's check to see if we should run a screensaver
   if (currApp->isAppMode(SCREENSAVER) && (pad.time() - screensaverStartedTime) > screensaverSwitchInterval) {
      launchApp(BritepadApp::SCREENSAVER_APP, SCREENSAVER);

    // is it time for the screensaver to kick in?
    } else if (!currApp->isAppMode(SCREENSAVER) && (pad.time() - pad.lastTouchedTime(ANY_PAD) > screensaverDelay)) {
      launchApp(BritepadApp::SCREENSAVER_APP, SCREENSAVER);
    }
  }

  setApp(getLaunchedApp(), getLaunchedAppMode());

  launchApp(BritepadApp::STAY_IN_APP);

  if (currApp) {
    currApp->run();
  } else {
    DEBUG_LN("currApp nullptr!");
  }

  // make sure the Timers get a chance to call their callbacks
  Timer::idle();
}
