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

  // if an app wants to be this mode, then give it a chance
  BritepadApp* wants = wantsToBe(m);
  if (wants) {
    return wants;
  }

  // count the enabled apps
  int count = 0;
  BritepadApp* nextapp = appList;
  while (nextapp) {
    if (nextapp->canBeAppMode(m) && ((nextapp)->getEnabled(m))) {
      count++;
    }
    nextapp = nextapp->getNextApp();
  }

  // pick a random one
  count = random(count);

  nextapp = appList;
  while (nextapp) {
    if (nextapp->canBeAppMode(m) && ((nextapp)->getEnabled(m))) {
      if (count == 0) {
        return nextapp;
      } else {
        count--;
      }
    }
    nextapp = nextapp->getNextApp();
  }
  DEBUGF("No random app avaialable for mode %d\n", m);
  return nullptr;
}

BritepadApp* Britepad::wantsToBe(AppMode m) {

  BritepadApp* nextapp = appList;
  while (nextapp) {
    if (nextapp->wantsToBe(m))
      return nextapp;
    nextapp = nextapp->getNextApp();
  }
  return nullptr;
}

void Britepad::addApp(BritepadApp* app) {
  if (appList) {
    appList->setPrevApp(app);
  }
  app->setNextApp(appList);
  appList = app;
}

BritepadApp* Britepad::getNextApp(BritepadApp* anApp) {
  if (anApp) {
    return anApp->getNextApp();
  } else {
    return appList;
  }
}

void Britepad::setApp(BritepadApp* newApp, AppMode asMode) {
  if (newApp == BritepadApp::STAY_IN_APP) {
    return;
  } else if (newApp == BritepadApp::MOUSE_APP) {
    newApp = randomApp(MOUSE_MODE);
    asMode = MOUSE_MODE;
  } else if (newApp == BritepadApp::SCREENSAVER_APP) {
    newApp = randomApp(SCREENSAVER_MODE);
    asMode = SCREENSAVER_MODE;
  } else if (newApp == BritepadApp::BACK_APP) {
    newApp = getAppByID(LauncherApp::ID);
    asMode = INTERACTIVE_MODE;
  }

  if (newApp) {
    launchedAppPtr = newApp;
  } else {
    DEBUG_LN("Can't switch to null newApp, returning to launcher");
    newApp = getAppByID(LauncherApp::ID);
    asMode = INTERACTIVE_MODE;
  }

  if (newApp == currApp) {
    if (currApp->getAppMode() != asMode) {
       currApp->setAppMode(asMode);
    }
    return;
  }

  if (currApp) {
    currApp->end();
  }

  currApp = newApp;

  if (currApp) {
    currApp->drawBars();
    currApp->begin();
    currApp->setAppMode(asMode);
    if (asMode == SCREENSAVER_MODE) {
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
    uint8_t light = max(Britepad::minBrightness,min( pad.getAmbientLight(), 255));

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
  launchApp(getAppByID(SplashApp::ID), SCREENSAVER_MODE);
  setApp(getAppByID(SplashApp::ID), SCREENSAVER_MODE);

// show the apps that have been loaded
  BritepadApp* anApp = appList;
  int count = 1;
  while (anApp != nullptr) {
//    DEBUG_LN(count);
//    DEBUG_LN(anApp->name());
//    DEBUG_LN((uint32_t)anApp);
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
//    DEBUG_LN("Toppad down");
    if (currApp) {
      BritepadApp* nextApp = currApp->exitsTo();
      launchApp(nextApp);
    } else {
      DEBUG_LN("No currapp!");
    }
  } else if (currApp->isAppMode(SCREENSAVER_MODE) && (pad.down(SCREEN_PAD) || ((pad.down(ANY_PAD) && !currApp->canBeInteractive())))) {
    DEBUG_LN("waking screensaver");
    // waking goes back to the mouse in the case that the user touched the screen (or any touch pad if it's not interactive)
    if (currApp->canBeMouse() && currApp->getEnabled(MOUSE_MODE)) {
      currApp->setAppMode(MOUSE_MODE);
    } else {
      launchApp(BritepadApp::MOUSE_APP, MOUSE_MODE);
    }

  } else if (pad.time() > disableScreensaversUntil && !currApp->disablesScreensavers()) {

    if ( pad.up(PROXIMITY_SENSOR) &&
         getAppByID(ClockApp::ID) &&
//       (getAppByID(ClockApp::ID))->getEnabled(SCREENSAVER_MODE) &&  // the proximity clock is always enabled (todo: make this a user pref)
         !currApp->isID(ClockApp::ID) &&
         currApp->isAppMode(SCREENSAVER_MODE) &&
         (pad.time() - pad.lastTouchedTime(ANY_PAD) > screensaverDelay))
    {
      launchApp(getAppByID(ClockApp::ID), SCREENSAVER_MODE);
      disableScreensavers(showClockDur);  // disable screensavers for a little while
      DEBUG_LN("Proximity detected: showing clock");

    } else {

     // check more often if somebody wants to be screensaver
     if (currApp->isAppMode(SCREENSAVER_MODE) &&
         (lastCheckWantsToBeScreensaver/checkWantsToBeScreensaverInterval != pad.time()/checkWantsToBeScreensaverInterval) &&
         !currApp->wantsToBe(SCREENSAVER_MODE) &&
         wantsToBe(SCREENSAVER_MODE)) {

        launchApp(BritepadApp::SCREENSAVER_APP, SCREENSAVER_MODE);

        lastCheckWantsToBeScreensaver = pad.time();
     }

      // let's check to see if we should run a screensaver
     if (currApp->isAppMode(SCREENSAVER_MODE) && (pad.time() - screensaverStartedTime) > screensaverSwitchInterval) {
        launchApp(BritepadApp::SCREENSAVER_APP, SCREENSAVER_MODE);

      // is it time for the screensaver to kick in?
      } else if (!currApp->isAppMode(SCREENSAVER_MODE) && (pad.time() - pad.lastTouchedTime(ANY_PAD) > screensaverDelay)) {
        launchApp(BritepadApp::SCREENSAVER_APP, SCREENSAVER_MODE);
      }
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
  sound.idle();
}
