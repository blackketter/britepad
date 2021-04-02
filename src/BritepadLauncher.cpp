#include "BritepadShared.h"

#include "BritepadLauncher.h"
#include "Timer.h"

#include "apps/LauncherApp.h"
#include "apps/ClockApp.h"
#include "apps/SplashApp.h"
#include "apps/AlarmApp.h"

#include "USB/USBHost.h"

BritepadEventQueue britepadEvents;
EventQueue* events = &britepadEvents;

// for the bell icon
#include "KeyIcons.h"

#define PROXIMITY_DEAD_TIME (1000)

void backlightCallback(void* data) {
  uint8_t lastBacklight = screen.getBacklight();

  // if we detect proximity or touch, it's probably casting a shadow and we don't want to update
  millis_t lastTouch = max(pad.lastTouchedTime(ANY_PAD), pad.lastTouchedTime(PROXIMITY_SENSOR));
  if ((pad.time() - lastTouch) > PROXIMITY_DEAD_TIME) {
    // any ambient light greater than 255 is full brightness, 1 is the minimums
    uint8_t light = max(BritepadLauncher::minBrightness,min( pad.getAmbientLight(), 255));

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

    // backlight not dimmable
    //screen.setBacklight(lastBacklight);
  }
}

void statusBarCallback(void* data) {
    ((BritepadLauncher*)data)->updateStatusBar();
}


void BritepadLauncher::begin() {

  screen.setBacklight(screen.maxbrightness);
  console.debugln("setBacklight begun");
  backlightTimer.setMillis(ambientUpdateInterval, backlightCallback, (void*)this, true);
  console.debugln("backlightTimer begun");
  statusBarUpdateTimer.setMillis(1000, statusBarCallback, (void*)this, true);
  console.debugln("statusBarUpdateTimer begun");

  usbHost.begin();
  console.debugln("usbHost begun");
  usbMouse.begin();
  console.debugln("usbMouse begun");

  Launcher::begin();
  console.debugln("superclass begun");

  // assumes that the splashapp has been created and added to list
  launchApp(SplashApp::ID);
  console.debugln("launchApp begun");

  // call superclass run() to initialize the splash app
  Launcher::run();

  currentBritepadApp()->setAppMode(INTERACTIVE_MODE);
}

void BritepadLauncher::idle() {
    millis_t now = Uptime::millis();
    if (now - lastIdle < idleInterval) {
      return;
    }

    Launcher::idle();
    watchdogKick();
    usbHost.Task();
    usbMouse.run();
    keys.idle();
    sound.idle();
    if (currentBritepadApp() && currentBritepadApp()->isAppMode(MOUSE_MODE)) {
      mousePad.run();
    };

    if (currentBritepadApp() && !currentBritepadApp()->usesKeyboard()) {
      events->sendKeys();
    }

    lastIdle = Uptime::millis();
};

void BritepadLauncher::exit() {
  if (currentBritepadApp()) {
    launchApp(currentBritepadApp()->exitsTo());
  }
}

void BritepadLauncher::launchApp(App* app) {
  BritepadLauncher::launchApp((BritepadApp*)app, INTERACTIVE_MODE);
};

void BritepadLauncher::launchApp(appid_t app) {
  BritepadLauncher::launchApp(app, INTERACTIVE_MODE);
};

void BritepadLauncher::launchApp(BritepadApp* app, AppMode mode) {
  if (currentBritepadApp()) {
    _lastAppMode = currentBritepadApp()->getAppMode();
  }
  Launcher::launchApp(app);
  _launchedAppMode = mode;
}

void BritepadLauncher::launchApp(appid_t id, AppMode mode) {
  if (currentBritepadApp()) {
    _lastAppMode = currentBritepadApp()->getAppMode();
  }
  Launcher::launchApp(id);
  _launchedAppMode = mode;
}

void BritepadLauncher::run() {
  pad.idle();

  if (pad.touched(ANY_PAD)) {
    resetScreensaver();
  }

  if (!currentBritepadApp() && !getLaunchedApp() ) {
      console.debugln("No curr app or launched app!");
      launchApp(LauncherApp::ID);
  }

  // an app has not been explicitly launched, let's see if something else should be launched
  if (!getLaunchedApp()) {

    if (pad.pressed(TOP_PAD)) {
       console.debugln("Menu touchpad (TOP_PAD) touched...");
       exit();

    } else if (currentBritepadApp()->isAppMode(SCREENSAVER_MODE) && (pad.pressed(SCREEN_PAD) ||
               ((pad.pressed(ANY_PAD) && !currentBritepadApp()->canBeInteractive())))) {
      console.debugln("waking screensaver");
      // waking goes back to the mouse in the case that the user touched the screen (or any touch pad if it's not interactive)
      if (currentBritepadApp()->canBeMouse() && currentBritepadApp()->getEnabled(MOUSE_MODE) && usbActive()) {
        console.debugln("switching current app to MOUSE_MODE");
        currentBritepadApp()->setAppMode(MOUSE_MODE);
      } else if (usbActive()) {
        console.debugln("launching A_MOUSE_APP");
        launchApp(BritepadApp::A_MOUSE_APP, MOUSE_MODE);
      } else {
        console.debugln("Waking to Launcher");
        launchApp(LauncherApp::ID);
      }

    // check if somebody wants to be screensaver
    } else if (currentBritepadApp()->isAppMode(SCREENSAVER_MODE) &&
       !currentBritepadApp()->wantsToRun() &&
       wantsToRun()) {

      console.debugln("launching a screensaver that wants to run");
      launchApp(BritepadApp::A_SCREENSAVER_APP, SCREENSAVER_MODE);

    // let's check to see if it's time run a screensaver
    } else if (Uptime::millis() > disableScreensaversUntil && !currentBritepadApp()->disablesScreensavers()) {

      if ( pad.pressed(PROXIMITY_SENSOR) &&
           currentBritepadApp()->isAppMode(SCREENSAVER_MODE) &&
           !currentBritepadApp()->displaysClock() &&
           BritepadApp::getAppByID(ClockApp::ID))
      {
        console.debugln("Proximity detected: showing clock");
        launchApp(ClockApp::ID, SCREENSAVER_MODE);
        resetScreensaver(showClockDur);  // disable screensavers for a little while
        sound.click();

      } else if ( (currentBritepadApp()->isAppMode(SCREENSAVER_MODE) || currentBritepadApp()->isAppMode(MOUSE_MODE)) &&
                  getScreensaverSwitchInterval() &&
                  (Uptime::millis() - screensaverStartedTime) > getScreensaverSwitchInterval()*1000 ) {
//          console.debugln("switching screensaver");
          launchApp(BritepadApp::A_SCREENSAVER_APP, SCREENSAVER_MODE);

        // is it time for the screensaver to kick in?
      } else if (!currentBritepadApp()->isAppMode(SCREENSAVER_MODE) && (Uptime::millis() > disableScreensaversUntil)
        && !(currentBritepadApp()->canBeScreensaver() && currentBritepadApp()->isAppMode(MOUSE_MODE))) {
        console.debugln("Launching screensaver");
        launchApp(BritepadApp::A_SCREENSAVER_APP, SCREENSAVER_MODE);
      }
    } else if (currentBritepadApp()->getAppMode() == INACTIVE_MODE) {
        // exiting!
        console.debugln("inactive, exiting");
        exit();
    }
  }

  BritepadApp* newApp = (BritepadApp*)getLaunchedApp();
  AppMode newMode = getLaunchedAppMode();

  if (newApp) {
    //  Handle the special references to app types
    if (newApp == BritepadApp::SWITCH_TO_INTERACTIVE_MODE) {
      newApp = currentBritepadApp();
      newMode= INTERACTIVE_MODE;
    } else if (newApp == BritepadApp::A_MOUSE_APP) {
      newApp = randomApp(MOUSE_MODE);
      newMode = MOUSE_MODE;
    } else if (newApp == BritepadApp::A_SCREENSAVER_APP) {
      newApp = randomApp(SCREENSAVER_MODE);
      newMode = SCREENSAVER_MODE;
    } else if (newApp == BritepadApp::LAST_APP) {
      newApp = (BritepadApp*)getLastApp();
      newMode = _lastAppMode;
    }

    if (newApp == BritepadApp::EXIT_APP) {
      newApp = (BritepadApp*)App::getAppByID(LauncherApp::ID);
      newMode = INTERACTIVE_MODE;
    }

    if (newMode == ANY_MODE) {
      newMode = INTERACTIVE_MODE;
    }

    if (newApp != getLaunchedApp()) {
      if (BritepadApp::validApp(newApp)) {
        launchApp(newApp, newMode);
//        console.debugf("launched new app: %s\n",newApp->name());
      } else {
        console.debugf("invalid app: %d\n", (int)newApp);
      }
    }
  }


  if (getLaunchedApp() && getLaunchedAppMode() == SCREENSAVER_MODE) {
    screensaverStartedTime = Uptime::millis();
  }

  if (getLaunchedApp() == currentBritepadApp() && currentBritepadApp()->getAppMode() != getLaunchedAppMode()) {
    currentBritepadApp()->setAppMode(newMode);
    console.debugln("switching app mode");
  }

  if (getLaunchedApp()) {
    ((BritepadApp*)getLaunchedApp())->setAppMode(getLaunchedAppMode());
    console.debugf("Launching app: %s, mode: %d\n", getLaunchedApp()->name(), getLaunchedAppMode());
  }

  Launcher::run();

  if (currentBritepadApp()->usesKeyboard()) {
    keys.idle();
    // when a keyboard app launches tell the host that all the keys have been released
    events->releaseKeys();
  }

}
BritepadApp* BritepadLauncher::wantsToRun() {

  BritepadApp* nextapp = BritepadApp::getFirstBritepadApp();
  while (nextapp) {
    if (nextapp->wantsToRun())
      return nextapp;
    nextapp = nextapp->getNextBritepadApp();
  }
  return nullptr;
}

BritepadApp* BritepadLauncher::randomApp(AppMode m) {

  // if an app wants to be this mode, then give it a chance
  BritepadApp* wants = wantsToRun();
  if (wants && wants->canBeAppMode(m)) {
    return wants;
  }

  // count the enabled apps
  int count = 0;
  BritepadApp* nextapp = BritepadApp::getFirstBritepadApp();
  while (nextapp) {
    if (nextapp->canBeAppMode(m) && ((nextapp)->getEnabled(m))) {
      count++;
    }
    nextapp = nextapp->getNextBritepadApp();
  }

  // pick a random one
  count = random(count);

  nextapp = BritepadApp::getFirstBritepadApp();
  while (nextapp) {
    if (nextapp->canBeAppMode(m) && ((nextapp)->getEnabled(m))) {
      if (count == 0) {
        return nextapp;
      } else {
        count--;
      }
    }
    nextapp = nextapp->getNextBritepadApp();
  }
  console.debugf("No random app avaialable for mode %d\n", m);
  return nullptr;
}

time_t BritepadLauncher::getScreensaverSwitchInterval() {
  time_t i = defaultScreensaverSwitchInterval;
  prefs.get(screensaverSwitchIntervalPref, sizeof(i), (uint8_t*)&i);
  return i;
}

void BritepadLauncher::setScreensaverSwitchInterval(time_t newInterval) {
   prefs.set(screensaverSwitchIntervalPref, sizeof(newInterval), (uint8_t*)&newInterval);
}

time_t BritepadLauncher::getScreensaverStartInterval() {
  time_t i = defaultScreensaverStartInterval;
  prefs.get(screensaverStartIntervalPref, sizeof(i), (uint8_t*)&i);
  return i;
}

void BritepadLauncher::setScreensaverStartInterval(time_t newInterval) {
   prefs.set(screensaverStartIntervalPref, sizeof(newInterval), (uint8_t*)&newInterval);
   resetScreensaver();
}

void BritepadLauncher::wakeHost() {
  // tap the shift key to wake the computer up
  if (events->keyIsUp(MODIFIERKEY_LEFT_SHIFT)) {
    Keyboard.press(KEY_RIGHT_ALT);
    Keyboard.release(KEY_RIGHT_ALT);
  }
  // jiggle the mouse to make cursor show up
  Mouse.move(-1, 0);
  Mouse.move(1, 0);

#if 0
  // send system wakeup
  // disabled now because sometimes it puts the system back to sleep
  Keyboard.press(KEY_SYSTEM_WAKE_UP);
  Keyboard.release(KEY_SYSTEM_WAKE_UP);
#endif
}

void BritepadLauncher::updateStatusBar() {
  // updates the time in the status bar
  drawStatusBar(true);
}

void BritepadLauncher::resetClipRect() {
  BritepadApp* curr = currentBritepadApp();
  if (curr) {
    coord_t top = curr->displaysStatusBar() ? _statusBarHeight : 0;
    coord_t bottom = curr->displaysInfoBar() ? screen.height()-_statusBarHeight : screen.height();
    curr->setClipRect(0, top, screen.width(), bottom-top);
  }
}

void BritepadLauncher::drawBars(bool update) {
  drawInfoBar(update);
  drawStatusBar(update);
}

void BritepadLauncher::drawStatusBar(bool update) {
  if (currentBritepadApp()->displaysStatusBar()) {
    screen.setClipRect(0, 0, screen.width(), _statusBarHeight);

    screen.setFont(&Arial_8_Bold);
    screen.setTextColor(currentBritepadApp()->statusBarFGColor());

    if (!update) {
      screen.fillScreen(currentBritepadApp()->statusBarBGColor());

      // draw title
      const char* title = currentBritepadApp()->statusBarTitle();
      screen.setCursor( (screen.clipWidth() - screen.measureTextWidth(title)) / 2,
                         (_statusBarHeight-screen.measureTextHeight(title)) / 2);
      screen.drawText(title);
    }

    // only include the clock if the app doesn't already draw a clock
    if (!currentBritepadApp()->displaysClock() && clock.hasBeenSet()) {
      // draw title
      screen.setFont(&Arial_8_Bold);
      color_t textColor = screen.mix(currentBritepadApp()->statusBarFGColor(), currentBritepadApp()->statusBarBGColor());
      screen.setTextColor(textColor, currentBritepadApp()->statusBarBGColor());
      char shortTime[20];
      clock.shortTime(shortTime);
      char shortTimeSpaced[100];
      sprintf(shortTimeSpaced,"  %s ", shortTime);
      screen.setCursor( (screen.clipRight() - screen.measureTextWidth(shortTimeSpaced) - 2),
                        (_statusBarHeight-screen.measureTextHeight(shortTimeSpaced)) / 2);
      screen.drawText(shortTimeSpaced);

      AlarmApp* alarm = (AlarmApp*)BritepadApp::getAppByID(AlarmApp::ID);
      if (alarm && alarm->getAlarmEnabled()) {
        Icon(bellIcon).draw( screen.clipRight() - screen.measureTextWidth(shortTimeSpaced) - 10, 4, textColor) ;
      }

      clock.shortDate(shortTime);
      sprintf(shortTimeSpaced,"  %s ", shortTime);
      screen.setCursor( screen.clipLeft() + 2,
                        (_statusBarHeight-screen.measureTextHeight(shortTimeSpaced)) / 2);
      screen.drawText(shortTimeSpaced);
    }

  }
  resetClipRect();
}

void BritepadLauncher::drawInfoBar(bool update) {
  if (currentBritepadApp()->displaysInfoBar()) {
    coord_t top = screen.height()-_statusBarHeight;
    screen.setClipRect(0, top, screen.width(), screen.height()-top);

    screen.setFont(&Arial_8_Bold);
    screen.setTextColor(currentBritepadApp()->infoBarFGColor());

    const char* text = currentBritepadApp()->infoBarText();
    if (!update && text) {
      screen.fillScreen(currentBritepadApp()->infoBarBGColor());

      screen.setCursor( (screen.clipWidth() - screen.measureTextWidth(text)) / 2,
                         top + (_statusBarHeight-screen.measureTextHeight(text)) / 2);
      screen.drawText(text);
    }

  }
   resetClipRect();
}

