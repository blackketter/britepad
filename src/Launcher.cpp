#include "Launcher.h"
#include "Types.h"
#include "Timer.h"
#include "Console.h"
extern Console console;

#include "TypeCommand.h"

extern KeyEventQueue* keyEvents;

#include "Dictionary.h"
extern EEPROMDictionary prefs;

#include "Commands/FPSCommand.h"
FPSCommand theFPSCommand;

App* Launcher::randomApp(AppMode m) {

  // if an app wants to be this mode, then give it a chance
  App* wants = wantsToRun();
  if (wants && wants->canBeAppMode(m)) {
    return wants;
  }

  // count the enabled apps
  int count = 0;
  App* nextapp = App::getFirstApp();
  while (nextapp) {
    if (nextapp->canBeAppMode(m) && ((nextapp)->getEnabled(m))) {
      count++;
    }
    nextapp = nextapp->getNextApp();
  }

  // pick a random one
  count = random(count);

  nextapp = App::getFirstApp();
  while (nextapp) {
    if (nextapp->canBeAppMode(m) && ((nextapp)->getEnabled(m))) {
      if (count == 0) {
        return (App*)nextapp;
      } else {
        count--;
      }
    }
    nextapp = nextapp->getNextApp();
  }
  console.debugf("No random app avaialable for mode %d\n", m);
  return nullptr;
}

App* Launcher::wantsToRun() {

  App* nextapp = App::getFirstApp();
  while (nextapp) {
    if (nextapp->wantsToRun())
      return (App*)nextapp;
    nextapp = nextapp->getNextApp();
  }
  return nullptr;
}

bool Launcher::setApp(App* newApp, AppMode asMode) {
  if (newApp == App::STAY_IN_APP) {
    return false;
  } else if (newApp == App::SWITCH_TO_INTERACTIVE_MODE) {
    currentApp()->switchAppMode(INTERACTIVE_MODE);
    return true;
  } else if (newApp == App::A_MOUSE_APP) {
    newApp = randomApp(MOUSE_MODE);
    asMode = MOUSE_MODE;
  } else if (newApp == App::A_SCREENSAVER_APP) {
    newApp = randomApp(SCREENSAVER_MODE);
    asMode = SCREENSAVER_MODE;
  } else if (newApp == App::LAST_APP) {
    newApp = lastAppPtr;
    asMode = lastAppMode;
  }

  if (newApp) {
    launchedAppPtr = newApp;
  } else {
    console.debugln("Can't switch to null newApp, bailing");
    return false;
  }

  if (asMode == ANY_MODE) {
    asMode = INTERACTIVE_MODE;
  }

  if (newApp == currentApp()) {
    if (currentApp()->getAppMode() != asMode) {
       currentApp()->switchAppMode(asMode);
    }
    return false;
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

    currentApp()->begin(asMode);
    }
  return true;
}

void Launcher::begin() {
  // initialize apps
  App* anApp = App::getFirstApp();
  while (anApp != nullptr) {
    anApp->init();
    anApp = anApp->getNextApp();
  }

}

void Launcher::idle() {
    millis_t now = Uptime::millis();
    if (now - lastIdle < idleInterval) {
      return;
    }
    if (currentApp() && !currentApp()->usesKeyboard()) {
      keyEvents->sendKeys();
    }
    // make sure the Timers get a chance to call their callbacks
    Timer::idle();
    console.idle();

    lastIdle = Uptime::millis();
};

// todo: sort the app list by priority instead of going through the list over and over again
 bool Launcher::event(KeyEvent* e) {
  bool consumed = false;
//  console.debugln("\n\n****BEGIN Processing event****");
  App* anApp = App::getFirstApp();
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
    anApp = App::getFirstApp();
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

void Launcher::loop() {
  setApp(getLaunchedApp(), getLaunchedAppMode());

  launchApp(App::STAY_IN_APP);

  currentApp()->run();
  theFPSCommand.newFrame();

  idle();
}

void Launcher::launchApp(App* app, AppMode mode) {
  if (app == App::LAST_APP) {
    launchedAppPtr = lastAppPtr;
    launchedAppMode = lastAppMode;
    lastAppPtr = nullptr;
  } else {
    launchedAppPtr = app;
    launchedAppMode = mode;
  }

  if (App::validApp(launchedAppPtr) &&
      launchedAppPtr->exitsTo() == App::LAST_APP
      ) {
    lastAppPtr = currentApp();
    if (lastAppPtr) {
      lastAppMode = currentApp()->getAppMode();
    }
    console.debugf("saving lastapp for %x\n", (uint32_t)app);
  } else if (launchedAppPtr != App::STAY_IN_APP){
    lastAppPtr = App::EXIT_APP;
//    console.debugf("NOT saving lastapp for %x\n", (uint32_t)app);
  }
}

void Launcher::launchApp(appid_t id, AppMode mode) {
  launchApp(App::getAppByID(id), mode);
}

void Launcher::exit() {
  if (currentApp()) {
    currentApp()->exit();
  }
}

time_t Launcher::getScreensaverSwitchInterval() {
  time_t i = defaultScreensaverSwitchInterval;
  prefs.get(screensaverSwitchIntervalPref, sizeof(i), (uint8_t*)&i);
  return i;
}

void Launcher::setScreensaverSwitchInterval(time_t newInterval) {
   prefs.set(screensaverSwitchIntervalPref, sizeof(newInterval), (uint8_t*)&newInterval);
}

time_t Launcher::getScreensaverStartInterval() {
  time_t i = defaultScreensaverStartInterval;
  prefs.get(screensaverStartIntervalPref, sizeof(i), (uint8_t*)&i);
  return i;
}

void Launcher::setScreensaverStartInterval(time_t newInterval) {
   prefs.set(screensaverStartIntervalPref, sizeof(newInterval), (uint8_t*)&newInterval);
   resetScreensaver();
}

class KeyEventsCommand : public Command {
  public:
    const char* getName() { return "events"; }
    const char* getHelp() { return "display keyboard events"; }
    void execute(Console* c, uint8_t paramCount, char** params) {
      keyEvents->printStatus(c);
    }
};

KeyEventsCommand theKeyEventsCommand;


