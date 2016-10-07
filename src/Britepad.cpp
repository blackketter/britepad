#include "BritepadShared.h"

#include "Britepad.h"
#include "Types.h"
#include "Timer.h"

#include "apps/LauncherApp.h"
#include "apps/ClockApp.h"
#include "apps/SplashApp.h"

#define DEBUG_ON 1
#include "Debug.h"

#define PROXIMITY_DEAD_TIME (1000)

BritepadApp* appList = nullptr;



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

void Britepad::sortApps() {
  BritepadApp* oldList = appList;
  appList = nullptr;

  while (oldList) {
    BritepadApp* i = oldList;
    BritepadApp* highest = i;
    while (i) {
      if (strcasecmp(i->name(), highest->name()) > 0) {
        highest = i;
      }
      i = i->getNextApp();
    }

    // remove from the list
    BritepadApp* p = highest->getPrevApp();
    BritepadApp* n = highest->getNextApp();
    if (p) { p->setNextApp(n); } else { oldList = n; }
    if (n) { n->setPrevApp(p); }

    addApp(highest);
  }
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
  } else if (newApp == BritepadApp::A_MOUSE_APP) {
    newApp = randomApp(MOUSE_MODE);
    asMode = MOUSE_MODE;
  } else if (newApp == BritepadApp::A_SCREENSAVER_APP) {
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
       currApp->switchAppMode(asMode);
    }
    return;
  }

  if (currApp) {
    currApp->end();
  }

  currApp = newApp;

  if (currApp) {
    currApp->drawBars();
    DEBUGF("Begin: %s\n", currApp->name());
    currApp->begin(asMode);
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
  // updates the time in the status bar
  currApp->drawStatusBar(true);
}

void Britepad::begin() {

  // we defer the creation of the launcher to guarantee it's last and all the other apps have been created
  theLauncherApp = new LauncherApp;

  // assumes that the splashapp has been created and added to list
  launchApp(getAppByID(SplashApp::ID), SCREENSAVER_MODE);
  setApp(getAppByID(SplashApp::ID), SCREENSAVER_MODE);

  // initialize apps
  BritepadApp* anApp = appList;
  int count = 0;
  while (anApp != nullptr) {
    anApp->init();
    anApp = anApp->getNextApp();
    count++;
  }

// show the apps that have been loaded
  DEBUGF("Total apps: %d\n", count);

  anApp = appList;
  count = 1;
  while (anApp != nullptr) {
    DEBUGF("  %d : %s (%08x)\n", count, anApp->name(), (uint32_t)anApp);
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
    if (currApp->canBeMouse() && currApp->getEnabled(MOUSE_MODE) && usbActive()) {
      currApp->switchAppMode(MOUSE_MODE);
    } else if (usbActive()) {
      launchApp(BritepadApp::A_MOUSE_APP, MOUSE_MODE);
    } else {
      launchApp(theLauncherApp);
    }

  } else if (pad.time() > disableScreensaversUntil && !currApp->disablesScreensavers()) {

    if ( pad.down(PROXIMITY_SENSOR) &&
         currApp->isAppMode(SCREENSAVER_MODE) &&

         !currApp->displaysClock() &&
//         !currApp->timeVisible() &&

         getAppByID(ClockApp::ID) &&
//       (getAppByID(ClockApp::ID))->getEnabled(SCREENSAVER_MODE) &&  // the proximity clock is always enabled (todo: make this a user pref)
         (pad.time() - pad.lastTouchedTime(ANY_PAD) > screensaverDelay))
    {
      launchApp(getAppByID(ClockApp::ID), SCREENSAVER_MODE);
      disableScreensavers(showClockDur);  // disable screensavers for a little while
      sound.click();
      DEBUG_LN("Proximity detected: showing clock");

    } else {

     // check more often if somebody wants to be screensaver
     if (currApp->isAppMode(SCREENSAVER_MODE) &&
         (lastCheckWantsToBeScreensaver/checkWantsToBeScreensaverInterval != pad.time()/checkWantsToBeScreensaverInterval) &&
         !currApp->wantsToBe(SCREENSAVER_MODE) &&
         wantsToBe(SCREENSAVER_MODE)) {

        launchApp(BritepadApp::A_SCREENSAVER_APP, SCREENSAVER_MODE);

        lastCheckWantsToBeScreensaver = pad.time();
     }

      // let's check to see if we should run a screensaver
     if (currApp->isAppMode(SCREENSAVER_MODE) && getScreensaverSwitchInterval() && (pad.time() - screensaverStartedTime) > getScreensaverSwitchInterval()*1000) {
        launchApp(BritepadApp::A_SCREENSAVER_APP, SCREENSAVER_MODE);

      // is it time for the screensaver to kick in?
      } else if (!currApp->isAppMode(SCREENSAVER_MODE) && (pad.time() - pad.lastTouchedTime(ANY_PAD) > screensaverDelay)) {
        launchApp(BritepadApp::A_SCREENSAVER_APP, SCREENSAVER_MODE);
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

time_t Britepad::getScreensaverSwitchInterval() {
  time_t i = defaultScreensaverSwitchInterval;
  prefs.read(screensaverSwitchIntervalPref, sizeof(i), (uint8_t*)&i);
  return i;
}

void Britepad::setScreensaverSwitchInterval(time_t newInterval) {
   prefs.write(screensaverSwitchIntervalPref, sizeof(newInterval), (uint8_t*)&newInterval);
}


void chimeCallback(void* data) {
  ((Britepad*)data)->chimerCallback();
}

void Britepad::resetChime() {
  if (clock.hasBeenSet()) {
    tmElements_t chimeTime;
    breakTime(now(), chimeTime);
    chimeTime.Minute = 0;
    chimeTime.Second = 0;
    chimeTime.Hour++;
    chimeTimer.setClockTime(makeTime(chimeTime), (timerCallback_t)chimeCallback, (void*)this);

    // how many chimes at the next hour
    chimesRemaining = hourFormat12() + 1;
    if (chimesRemaining == 13) { chimesRemaining = 1; }
  }
}


void Britepad::chimerCallback() {
  if (chimesRemaining == 0) {
    resetChime();
  } else {
    chimeTimer.setMillis(chimeInterval, (timerCallback_t)chimeCallback, (void*)this);
    sound.beep();
    chimesRemaining--;
  }
}
