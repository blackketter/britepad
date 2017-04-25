#include "BritepadShared.h"

#include "Britepad.h"
#include "Types.h"
#include "Timer.h"

#include "apps/LauncherApp.h"
#include "apps/ClockApp.h"
#include "apps/SplashApp.h"

#define PROXIMITY_DEAD_TIME (1000)

class FPSCommand : public Command {
  public:
    const char* getName() { return "fps"; }
    const char* getHelp() { return "toggle displaying frame rate in fps"; }
    void execute(Stream* c, uint8_t paramCount, char** params) {
      enable = !enable;
      c->printf("FPS display %s\n", enable ? "enabled" : "disabled");
    }
    void newFrame() {
      frames++;
      millis_t now = Uptime::millis();
      millis_t frameDur = now - lastFrame;
      if (frameDur > maxFrame) { maxFrame = frameDur; }
      if (now - lastTime > 1000) {
        if (enable) {
          console.debugf("FPS: %.2f (Max frame: %dms, Max idle: %dms)\n", ((float)(frames*1000))/(now - lastTime), maxFrame, maxIdle);
        }
        lastTime = now;
        frames = 0;
        maxFrame = 0;
        maxIdle = 0;
      }
      lastFrame = now;
    }

    void newIdle() {
      millis_t now = Uptime::millis();
      millis_t idleDur = now - lastIdle;
      if (idleDur > maxIdle) { maxIdle = idleDur; }
      lastIdle = now;
    }

  private:
    bool enable = false;
    uint32_t frames = 0;
    millis_t lastTime;
    millis_t lastFrame;
    millis_t maxFrame;

    millis_t lastIdle;
    millis_t maxIdle;
};
FPSCommand theFPSCommand;

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
  BritepadApp* wants = wantsToRun();
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
  console.debugf("No random app avaialable for mode %d\n", m);
  return nullptr;
}

BritepadApp* Britepad::wantsToRun() {

  BritepadApp* nextapp = appList;
  while (nextapp) {
    if (nextapp->wantsToRun())
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
    console.debugln("Can't switch to null newApp, returning to launcher");
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
    console.debugf("Begin: %s\n", currApp->name());
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
  while (anApp != nullptr) {
    anApp->init();
    anApp = anApp->getNextApp();
  }

  screen.fillScreen(screen.black);
  screen.setBacklight(screen.maxbrightness);
  backlightTimer.setMillis(ambientUpdateInterval, backlightCallback, (void*)this, true);
  statusBarUpdateTimer.setMillis(1000, statusBarCallback, (void*)this, true);
}

void Britepad::idle() {
  if (currApp && !currApp->usesKeyboard()) {
    theFPSCommand.newIdle();
    keyMatrix.update();
  }

  idleApps();

  if (currApp && !currApp->usesKeyboard()) {
    keyMatrix.sendKeys();
  }
};

void Britepad::idleApps() {
  // idle apps
  BritepadApp* anApp = appList;
  while (anApp != nullptr) {
    anApp->idle();
    anApp = anApp->getNextApp();
  }
}

void Britepad::loop() {

  theFPSCommand.newIdle();
  keyMatrix.update();
  pad.update();
  idleApps();

  if (pad.down(TOP_PAD)) {
//    console.debugln("Toppad down");
    if (currApp) {
      BritepadApp* nextApp = currApp->exitsTo();
      launchApp(nextApp);
    } else {
      console.debugln("No currapp!");
    }
  } else if (currApp->isAppMode(SCREENSAVER_MODE) && (pad.down(SCREEN_PAD) || ((pad.down(ANY_PAD) && !currApp->canBeInteractive())))) {
    console.debugln("waking screensaver");
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

         (pad.time() - pad.lastTouchedTime(ANY_PAD) > screensaverDelay))
    {
      launchApp(theLauncherApp);
      console.debugln("Proximity detected: showing launcher");

    } else if ( pad.down(PROXIMITY_SENSOR) &&
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
      console.debugln("Proximity detected: showing clock");

    } else {

     // check more often if somebody wants to be screensaver
     if (currApp->isAppMode(SCREENSAVER_MODE) &&
         (lastCheckWantsToBeScreensaver/checkWantsToBeScreensaverInterval != pad.time()/checkWantsToBeScreensaverInterval) &&
         !currApp->wantsToRun() &&
         wantsToRun()) {

        launchApp(BritepadApp::A_SCREENSAVER_APP, SCREENSAVER_MODE);

        lastCheckWantsToBeScreensaver = pad.time();
     }

      // let's check to see if we should run a screensaver
     if (currApp->isAppMode(SCREENSAVER_MODE) && getScreensaverSwitchInterval() && (pad.time() - screensaverStartedTime) > getScreensaverSwitchInterval()*1000) {
        launchApp(BritepadApp::A_SCREENSAVER_APP, SCREENSAVER_MODE);

      // is it time for the screensaver to kick in?
      } else if (!currApp->isAppMode(SCREENSAVER_MODE) && (pad.time() - pad.lastTouchedTime(ANY_SENSOR) > screensaverDelay)) {
        launchApp(BritepadApp::A_SCREENSAVER_APP, SCREENSAVER_MODE);
      }
    }

  }

  setApp(getLaunchedApp(), getLaunchedAppMode());

  launchApp(BritepadApp::STAY_IN_APP);

  if (currApp) {
    currApp->run();
    theFPSCommand.newFrame();
  } else {
    console.debugln("currApp nullptr!");
  }

  // make sure the Timers get a chance to call their callbacks
  Timer::idle();
  sound.idle();
  console.idle();

  keyMatrix.sendKeys();
}

time_t Britepad::getScreensaverSwitchInterval() {
  time_t i = defaultScreensaverSwitchInterval;
  prefs.read(screensaverSwitchIntervalPref, sizeof(i), (uint8_t*)&i);
  return i;
}

void Britepad::setScreensaverSwitchInterval(time_t newInterval) {
   prefs.write(screensaverSwitchIntervalPref, sizeof(newInterval), (uint8_t*)&newInterval);
}

