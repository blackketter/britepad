#include "Launcher.h"
#include "Types.h"
#include "Timer.h"
#include "Console.h"
extern Console console;

#include "TypeCommand.h"

#include "Dictionary.h"
extern EEPROMDictionary prefs;

#include "Commands/FPSCommand.h"
FPSCommand theFPSCommand;

void Launcher::begin() {
  // initialize apps
  App* anApp = App::getFirstApp();
  while (anApp != nullptr) {
    anApp->init();
    anApp = anApp->getNextApp();
  }
}

void Launcher::idle() {
}

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

void Launcher::run() {
  //  Do we have a new app to launch
  if (_launchedApp) {
    if (_launchedApp != _currApp) {
      // end the old app
      if (_currApp) {
        _currApp->end();
      }
      // Launch it
      _lastApp = currentApp();
      _currApp = _launchedApp;

      console.debugf("Launching app: %s\n", _currApp->name());
      _currApp->begin();
    }
    _launchedApp = nullptr;
  }

// Run it
  _currApp->run();

// Idle system
  idle();

  theFPSCommand.newFrame();
}

void Launcher::launchApp(App* app) {
  if (app) {
    _launchedApp = app;
//    console.debugf("Launching app: %d\n", (int)app);
  } else {
    console.debugln("Can't switch to null newApp, bailing");
  }
}

void Launcher::launchApp(appid_t id) {
//  console.debugf("launching app id: %s\n", id);
  launchApp(App::getAppByID(id));
}

