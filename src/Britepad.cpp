#include "BritepadShared.h"

#include "Britepad.h"
#include "Types.h"
#include "Timer.h"

#include "apps/LauncherApp.h"
#include "apps/ClockApp.h"
#include "apps/SplashApp.h"
#include "apps/AlarmApp.h"

#include "USB/USBHost.h"

#include "Commands/FPSCommand.h"
FPSCommand theFPSCommand;

BritepadKeyEventQueue keyEvents;
#include "TypeCommand.h"

// for the bell icon
#include "KeyIcons.h"

#define PROXIMITY_DEAD_TIME (1000)

class QuitCommand : public Command {
  const char* getName() { return "quit"; }
  const char* getHelp() { return "exit the current app"; }
  void execute(Console* c, uint8_t paramCount, char** params) {
    britepad.currentApp()->exit();
  }
};
QuitCommand theQuitCommand;

BritepadApp* Britepad::randomApp(AppMode m) {

  // if an app wants to be this mode, then give it a chance
  BritepadApp* wants = wantsToRun();
  if (wants && wants->canBeAppMode(m)) {
    return wants;
  }

  // count the enabled apps
  int count = 0;
  BritepadApp* nextapp = BritepadApp::getFirstApp();
  while (nextapp) {
    if (nextapp->canBeAppMode(m) && ((nextapp)->getEnabled(m))) {
      count++;
    }
    nextapp = nextapp->getNextApp();
  }

  // pick a random one
  count = random(count);

  nextapp = BritepadApp::getFirstApp();
  while (nextapp) {
    if (nextapp->canBeAppMode(m) && ((nextapp)->getEnabled(m))) {
      if (count == 0) {
        return (BritepadApp*)nextapp;
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

  BritepadApp* nextapp = BritepadApp::getFirstApp();
  while (nextapp) {
    if (nextapp->wantsToRun())
      return (BritepadApp*)nextapp;
    nextapp = nextapp->getNextApp();
  }
  return nullptr;
}

void Britepad::setApp(BritepadApp* newApp, AppMode asMode) {
  if (newApp == BritepadApp::STAY_IN_APP) {
    return;
  } else if (newApp == BritepadApp::SWITCH_TO_INTERACTIVE_MODE) {
    currentApp()->switchAppMode(INTERACTIVE_MODE);
    return;
  } else if (newApp == BritepadApp::A_MOUSE_APP) {
    newApp = randomApp(MOUSE_MODE);
    asMode = MOUSE_MODE;
  } else if (newApp == BritepadApp::A_SCREENSAVER_APP) {
    newApp = randomApp(SCREENSAVER_MODE);
    asMode = SCREENSAVER_MODE;
  } else if (newApp == BritepadApp::LAST_APP) {
    newApp = lastAppPtr;
    asMode = lastAppMode;
  } else if (newApp == BritepadApp::EXIT_APP) {
    newApp = BritepadApp::getAppByID(LauncherApp::ID);
    asMode = INTERACTIVE_MODE;
  }

  if (newApp) {
    launchedAppPtr = newApp;
  } else {
    console.debugln("Can't switch to null newApp, returning to launcher");
    newApp = BritepadApp::getAppByID(LauncherApp::ID);
    asMode = INTERACTIVE_MODE;
  }

  if (asMode == ANY_MODE) {
    asMode = INTERACTIVE_MODE;
  }

  if (newApp == currentApp()) {
    if (currentApp()->getAppMode() != asMode) {
       currentApp()->switchAppMode(asMode);
    }
    return;
  }

  if (asMode == SCREENSAVER_MODE) {
    screensaverStartedTime = pad.time();
  }

  if (currentApp()) {
    currentApp()->end();
  }

  setCurrentApp(newApp);

  if (currentApp()) {
    const char* modeString = "unknown";
    switch (asMode) {
      case SCREENSAVER_MODE:
        modeString = "screensaver";
        break;
      case INTERACTIVE_MODE:
        modeString = "interactive";
        break;
      case MOUSE_MODE:
        modeString = "mouse";
        break;
      default:
        break;
    }
    console.debugf("Begin: %s in %s mode\n", currentApp()->name(), modeString);
    if (currentApp()->usesKeyboard()) {
      keys.update();

      // when a keyboard app launches tell the host that all the keys have been released
      keyEvents.releaseKeys();
    }

    currentApp()->begin(asMode);
    drawBars();
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

    // backlight not dimmable
    //screen.setBacklight(lastBacklight);
  }
}

void statusBarCallback(void* data) {
    ((Britepad*)data)->updateStatusBar();
}

void Britepad::updateStatusBar() {
  // updates the time in the status bar
  drawStatusBar(true);
}

void Britepad::resetClipRect() {
//TODO - Remove need for this cast
  BritepadApp* curr = (BritepadApp*)currentApp();
  if (curr) {
    coord_t top = curr->displaysStatusBar() ? _statusBarHeight : 0;
    coord_t bottom = curr->displaysInfoBar() ? screen.height()-_statusBarHeight : screen.height();
    curr->setClipRect(0, top, screen.width(), bottom-top);
  }
}
void Britepad::begin() {

  // assumes that the splashapp has been created and added to list
  launchApp(SplashApp::ID, SCREENSAVER_MODE);
  setApp(BritepadApp::getAppByID(SplashApp::ID), SCREENSAVER_MODE);

  // initialize apps
  BritepadApp* anApp = BritepadApp::getFirstApp();
  while (anApp != nullptr) {
    anApp->init();
    anApp = anApp->getNextApp();
  }

  screen.setBacklight(screen.maxbrightness);
  backlightTimer.setMillis(ambientUpdateInterval, backlightCallback, (void*)this, true);
  statusBarUpdateTimer.setMillis(1000, statusBarCallback, (void*)this, true);

	usbHost.begin();
	usbMouse.begin();
}

void Britepad::idle() {
    millis_t now = Uptime::millis();
    if (now - lastIdle < idleInterval) {
      return;
    }

    watchdogKick();

    usbHost.Task();
    usbMouse.run();
    keys.update();

    if (currentApp() && !currentApp()->usesKeyboard()) {
      keyEvents.sendKeys();
    }
    if (currentApp() && currentApp()->isAppMode(MOUSE_MODE)) {
      mousePad.run();
    };  // run current app state repeatedly

    // make sure the Timers get a chance to call their callbacks
    Timer::idle();
    sound.idle();
    console.idle();

    lastIdle = now;
};

// todo: sort the app list by priority instead of going through the list over and over again
 bool Britepad::event(KeyEvent* e) {
  bool consumed = false;
//  console.debugln("\n\n****BEGIN Processing event****");
  BritepadApp* anApp = BritepadApp::getFirstApp();
  EventPriority lowestPriority = PRIORITY_NORMAL;
  EventPriority currentPriority = PRIORITY_NORMAL;

  // find the lowest priority to start
  while (anApp != nullptr) {
    currentPriority = anApp->eventPriority();
//    console.debugf("Priority: %s %d\n", anApp->name(), currentPriority);

    if (currentPriority < lowestPriority) {
      lowestPriority = currentPriority;
    }
    anApp = anApp->getNextApp();
  }


  // iterate through the priorities in order, starting with the lowest
  currentPriority = lowestPriority;
  EventPriority nextPriority = currentPriority;

  do {
    anApp = BritepadApp::getFirstApp();
//    console.debugf("***Current Priority: %d\n", currentPriority);
    // go through list looking for the current priority and process those
    while (anApp != nullptr) {
      EventPriority appPriority = anApp->eventPriority();
//      console.debugf("Checking: %s %d\n", anApp->name(), appPriority);

      if (appPriority == currentPriority) {
        // if this app is eating the event, then return immediately
//        console.debugln("  ---Processing event");
        consumed = anApp->event(e);
        if (consumed) {
//            console.debugln("  EVENT CONSUMED");
            break;
        }

      } else if (appPriority > currentPriority){
        if ((nextPriority == currentPriority) || (appPriority < nextPriority)) {
          nextPriority = appPriority;
        }
      }
      anApp = anApp->getNextApp();
    }
    if (consumed) {
      break;
    }
    if (nextPriority == currentPriority) {
      break;
    } else {
      currentPriority = nextPriority;
    }

  } while (true);
  return consumed;
}

void Britepad::loop() {

  pad.update();

  if (pad.touched(ANY_PAD)) {
    resetScreensaver();
  }

  if (!currentApp()) {
      console.debugln("No currapp!");
      launchApp(LauncherApp::ID);
  }

  if (pad.pressed(TOP_PAD)) {
    currentApp()->exit();
  } else if (currentApp()->isAppMode(SCREENSAVER_MODE) && (pad.pressed(SCREEN_PAD) || ((pad.pressed(ANY_PAD) && !currentApp()->canBeInteractive())))) {
    console.debugln("waking screensaver");
    // waking goes back to the mouse in the case that the user touched the screen (or any touch pad if it's not interactive)
    if (currentApp()->canBeMouse() && currentApp()->getEnabled(MOUSE_MODE) && usbActive()) {
      console.debugln("switching current app to MOUSE_MODE");
      currentApp()->switchAppMode(MOUSE_MODE);
    } else if (usbActive()) {
      console.debugln("launching A_MOUSE_APP");
      launchApp(BritepadApp::A_MOUSE_APP, MOUSE_MODE);
    } else {
      launchApp(LauncherApp::ID);
    }

  } else  {
   // check if somebody wants to be screensaver
   if (currentApp()->isAppMode(SCREENSAVER_MODE) &&
       !currentApp()->wantsToRun() &&
       wantsToRun()) {

      launchApp(BritepadApp::A_SCREENSAVER_APP, SCREENSAVER_MODE);

  // let's check to see if we should run a screensaver
  } else if (pad.time() > disableScreensaversUntil && !currentApp()->disablesScreensavers()) {

    if ( pad.pressed(PROXIMITY_SENSOR) &&
         currentApp()->isAppMode(SCREENSAVER_MODE) &&
         !currentApp()->displaysClock() &&
         BritepadApp::getAppByID(ClockApp::ID))
    {
      launchApp(ClockApp::ID, SCREENSAVER_MODE);
      resetScreensaver(showClockDur);  // disable screensavers for a little while
      sound.click();
      console.debugln("Proximity detected: showing clock");

    } else if (currentApp()->isAppMode(SCREENSAVER_MODE) && getScreensaverSwitchInterval() && (pad.time() - screensaverStartedTime) > getScreensaverSwitchInterval()*1000) {
        launchApp(BritepadApp::A_SCREENSAVER_APP, SCREENSAVER_MODE);

      // is it time for the screensaver to kick in?
      } else if (!currentApp()->isAppMode(SCREENSAVER_MODE) && (pad.time() > disableScreensaversUntil)
        && !(currentApp()->canBeScreensaver() && currentApp()->isAppMode(MOUSE_MODE))) {
        launchApp(BritepadApp::A_SCREENSAVER_APP, SCREENSAVER_MODE);
      }
    }
  }

  setApp(getLaunchedApp(), getLaunchedAppMode());

  launchApp(BritepadApp::STAY_IN_APP);

  currentApp()->run();
  theFPSCommand.newFrame();

  idle();
}

void Britepad::launchApp(BritepadApp* app, AppMode mode) {
  if (app == BritepadApp::LAST_APP) {
    launchedAppPtr = lastAppPtr;
    launchedAppMode = lastAppMode;
    lastAppPtr = nullptr;
  } else {
    launchedAppPtr = app;
    launchedAppMode = mode;
  }

  if (BritepadApp::validApp(launchedAppPtr) &&
      launchedAppPtr->exitsTo() == BritepadApp::LAST_APP
      ) {
    lastAppPtr = currentApp();
    if (lastAppPtr) {
      lastAppMode = currentApp()->getAppMode();
    }
    console.debugf("saving lastapp for %x\n", (uint32_t)app);
  } else if (launchedAppPtr != BritepadApp::STAY_IN_APP){
    lastAppPtr = BritepadApp::EXIT_APP;
//    console.debugf("NOT saving lastapp for %x\n", (uint32_t)app);
  }
}

void Britepad::launchApp(appid_t id, AppMode mode) {
  launchApp(BritepadApp::getAppByID(id), mode);
}

void Britepad::exit() {
  if (currentApp()) {
    currentApp()->exit();
  }
}

time_t Britepad::getScreensaverSwitchInterval() {
  time_t i = defaultScreensaverSwitchInterval;
  prefs.get(screensaverSwitchIntervalPref, sizeof(i), (uint8_t*)&i);
  return i;
}

void Britepad::setScreensaverSwitchInterval(time_t newInterval) {
   prefs.set(screensaverSwitchIntervalPref, sizeof(newInterval), (uint8_t*)&newInterval);
}

time_t Britepad::getScreensaverStartInterval() {
  time_t i = defaultScreensaverStartInterval;
  prefs.get(screensaverStartIntervalPref, sizeof(i), (uint8_t*)&i);
  return i;
}

void Britepad::setScreensaverStartInterval(time_t newInterval) {
   prefs.set(screensaverStartIntervalPref, sizeof(newInterval), (uint8_t*)&newInterval);
   resetScreensaver();
}

void Britepad::wakeHost() {
  // tap the shift key to wake the computer up
  if (keyEvents.keyIsUp(MODIFIERKEY_LEFT_SHIFT)) {
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

void Britepad::drawBars(bool update) {
  drawInfoBar(update);
  drawStatusBar(update);
}

void Britepad::drawStatusBar(bool update) {
  if (currentApp()->displaysStatusBar()) {
    screen.setClipRect(0, 0, screen.width(), _statusBarHeight);

    screen.setFont(&Arial_8_Bold);
    screen.setTextColor(currentApp()->statusBarFGColor());

    if (!update) {
      screen.fillScreen(currentApp()->statusBarBGColor());

      // draw title
      const char* title = currentApp()->statusBarTitle();
      screen.setCursor( (screen.clipWidth() - screen.measureTextWidth(title)) / 2,
                         (_statusBarHeight-screen.measureTextHeight(title)) / 2);
      screen.drawText(title);
    }

    // only include the clock if the app doesn't already draw a clock
    if (!currentApp()->displaysClock() && clock.hasBeenSet()) {
      // draw title
      screen.setFont(&Arial_8_Bold);
      color_t textColor = screen.mix(currentApp()->statusBarFGColor(), currentApp()->statusBarBGColor());
      screen.setTextColor(textColor, currentApp()->statusBarBGColor());
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

void Britepad::drawInfoBar(bool update) {
  if (currentApp()->displaysInfoBar()) {
    coord_t top = screen.height()-_statusBarHeight;
    screen.setClipRect(0, top, screen.width(), screen.height()-top);

    screen.setFont(&Arial_8_Bold);
    screen.setTextColor(currentApp()->infoBarFGColor());

    const char* text = currentApp()->infoBarText();
    if (!update && text) {
      screen.fillScreen(currentApp()->infoBarBGColor());

      screen.setCursor( (screen.clipWidth() - screen.measureTextWidth(text)) / 2,
                         top + (_statusBarHeight-screen.measureTextHeight(text)) / 2);
      screen.drawText(text);
    }

  }
   resetClipRect();
}


class KeyEventsCommand : public Command {
  public:
    const char* getName() { return "events"; }
    const char* getHelp() { return "display keyboard events"; }
    void execute(Console* c, uint8_t paramCount, char** params) {
      keyEvents.printStatus(c);
    }
};

KeyEventsCommand theKeyEventsCommand;
