#include "BritepadShared.h"

#include "Britepad.h"
#include "Types.h"
#include "Timer.h"

#include "Debug.h"

// apps are included here
#include "BritepadApp.h"
#include "ScreensaverApp.h"
#include "BubblesApp.h"
#include "MouseApp.h"
#include "LauncherApp.h"
#include "SplashApp.h"
#include "PassApp.h"
#include "KeyApp.h"
#include "SetClockApp.h"
#include "ClockApp.h"
#include "DotsDisplayApp.h"
#include "SetTimerApp.h"
#include "MuteApp.h"
#include "ThereminApp.h"
#include "StopwatchApp.h"

Britepad::Britepad(void) {

}

ScreensaverApp* Britepad::randomScreensaver(void) {
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


void Britepad::setApp(BritepadApp* newApp) {
  if (newApp == 0) {
    DEBUG_LN("Set currApp to NIL!");
    return;
  }

  if (newApp == currApp) {
    return;
  }

  if (currApp)
    currApp->end(newApp);

  currApp = newApp;

  if (currApp) {
    currApp->updateStatusBar(false);  // update the status bar clipping
    currApp->begin();
    currApp->updateStatusBar(true);   // draw the status bar
  }
}



void Britepad::begin(void) {

// these are special apps that are shared across the enviroment
  launcherApp = new LauncherApp;
  defaultApp = new MouseApp;

  ScreensaverApp* splashApp = new SplashApp;
  ScreensaverApp* bubblesApp = new BubblesApp;

// left screen contains screensavers and settings
  launcherApp->setButton(0, 0,  bubblesApp);
  launcherApp->setButton(0, 1,  splashApp);
  launcherApp->setButton(0, 2,  new DotsDisplayApp);
  launcherApp->setButton(0, 3,  new ClockApp);
  launcherApp->setButton(0, 8,  new SetClockApp);
  launcherApp->setButton(0, 9,  new MuteApp);

// middle screen has quick buttons
  launcherApp->setButton(1, 0,  new KeyApp("Vol+", KEY_MEDIA_VOLUME_INC, screen.bluegreen));
  launcherApp->setButton(1, 4,  new KeyApp("Vol-", KEY_MEDIA_VOLUME_DEC, screen.bluegreen));
  launcherApp->setButton(1, 8,  new KeyApp("Mute", KEY_MEDIA_MUTE, screen.blue));

  launcherApp->setButton(1, 1,  new KeyApp("<<", KEY_MEDIA_PREV_TRACK, screen.orange));
  launcherApp->setButton(1, 2,  new KeyApp("||", KEY_MEDIA_PLAY_PAUSE, screen.orange));
  launcherApp->setButton(1, 3,  new KeyApp(">>", KEY_MEDIA_NEXT_TRACK, screen.orange));

// i never use this
//  launcherApp->setButton(1, 7,  new KeyApp("Eject", KEY_MEDIA_EJECT));

  launcherApp->setButton(1, 10, new PassApp("iCloud", "Bu77cracks!\n") );
  launcherApp->setButton(1, 11, new PassApp("Pass", "ch@db1ldr3n\n") );

// right screen has useful apps
  launcherApp->setButton(2, 0, new SetTimerApp("10 sec", 10));
  launcherApp->setButton(2, 1, new SetTimerApp("3 min", 3*60));
  launcherApp->setButton(2, 2, new SetTimerApp("25 min", 25*60));
  launcherApp->setButton(2, 3, new SetTimerApp("55 min", 55*60));

  launcherApp->setButton(2, 8,  new ThereminApp);

// just for testing
//  launcherApp->setButton(2, 9,  new KeyApp("My Name", "Dean\nBlackketter"));

  launcherApp->setButton(2, 11, new StopwatchApp);

// show the splash screen
  setApp(splashApp);

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
    // start launcher
    if (currApp != launcherApp) {
      setApp(launcherApp);
      pad.update();  // consume that down event
    }
  } else if (pad.touched(ANY_PAD)) {
    // touching resets screensaver
    if (currApp->isScreensaver()) {
      setApp(defaultApp);
    }
  } else if ((pad.time() - pad.lastUpTime(ANY_PAD) > screensaverDelay)) {
    // time to run a screensaver

    // do we have somebody who wants to be screensaver?
    if (BritepadApp *wanter = wantsToBeScreensaver()) {
      setApp(wanter);

    // are we not running a screensaver?
    } else if (!currApp->isScreensaver()){
      setApp(randomScreensaver());
      screensaverStartedTime = pad.time();

    // is it time to switch?
    } else if (pad.time() - screensaverStartedTime > screensaverSwitchInterval) {
      setApp(randomScreensaver());
      screensaverStartedTime = pad.time();
    }
  }


  // run the app
  if (currApp) {

    BritepadApp* newApp = currApp->run();

    if (newApp == BritepadApp::DEFAULT_APP) {
      newApp = defaultApp;
    } else if (newApp == BritepadApp::BACK_APP) {
      newApp = launcherApp;
    }

    if (newApp) {
      setApp(newApp);
    }

  } else {
    DEBUG_LN("currApp nil!");
  }

  // make sure the Timers get a chance to call their callbacks
  Timer::idle();

}
