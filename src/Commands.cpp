#include "BritepadShared.h"
#include "BritepadLauncher.h"
#include "apps/BritepadApp.h"

class AppsCommand : public Command {
  public:
    const char* getName() { return "apps"; }
    const char* getHelp() { return "Lists installed apps"; }
    void execute(Console* c, uint8_t paramCount, char** params) {
        // show the apps that have been loaded
        App* anApp = App::getFirstApp();
        int count = 1;
        while (anApp != nullptr) {
          c->printf("  %d : %s - %s (%08x)\n", count, anApp->id(), anApp->name(), (uint32_t)anApp);
          anApp = anApp->getNextApp();
          count++;
        }
    }
};
AppsCommand theAppsCommand;

// TODO: Make this not BritepadApp specific
class AppCommand : public Command {
  public:
    const char* getName() { return "app"; }
    const char* getHelp() { return "appid | index - Print app info, default current"; }
    void execute(Console* c, uint8_t paramCount, char** params) {
      BritepadApp* anApp = nullptr;
      if (paramCount) {
        anApp = (BritepadApp*)App::findApp(params[1]);
      }

      if (anApp == nullptr) {
        anApp = (BritepadApp*)launcher.currentApp();
        c->print("Current ");
      }

      // show the apps that have been loaded
       if (anApp) {
        String modeString;
        c->printf("App:\n  name: %s\n  id:   %s\n  addr: %08x\n", anApp->name(), anApp->id(), (uint32_t)anApp);
        switch (anApp->getAppMode()) {
          case MOUSE_MODE:
            modeString = "MOUSE_MODE";
            break;
          case SCREENSAVER_MODE:
            modeString = "SCREENSAVER_MODE";
            break;
          case INTERACTIVE_MODE:
            modeString = "INTERACTIVE_MODE";
            break;
          default:
            modeString += anApp->getAppMode(); // string append
        };
        c->printf("  mode: %s\n", modeString.c_str());
      } else {
        c->printf("No currently running app!\n");
      }
    }
};
AppCommand theAppCommand;

// TODO: Make this not BritepadApp specific
class LaunchCommand : public Command {
  public:
    const char* getName() { return "launch"; }
    const char* getHelp() { return "[-s] appid | index - Launch a given app, -s for screensaver"; }
    void execute(Console* c, uint8_t paramCount, char** params) {
      uint8_t pathindex = 1;
      AppMode mode = INTERACTIVE_MODE;

      if (paramCount && (params[1][0] == '-')) {
        if (strchr(params[1], 's')) {
            mode = SCREENSAVER_MODE;
        }
        pathindex++;
      }

        App* a = App::findApp(params[pathindex]);
//        c->printf("got back app %s - %x\n", params[pathindex],(uint32_t)a);
        if (a) {
          c->printf("Running app '%s' - %s...\n",a->id(), a->name());
          launcher.launchApp((BritepadApp*)a, mode);
          launcher.resetScreensaver(5*60*1000);  // run it for 5 minutes, then let the screensavers do their work
        } else {
          c->println("Invalid app id");
          printError(c);
        }
      }
};
LaunchCommand theLaunchCommand;

class PrefsCommand : public Command {
  public:
    const char* getName() { return "prefs"; }
    const char* getHelp() { return "dump prefs data"; }
    void execute(Console* c, uint8_t paramCount, char** params) {
      const uint8_t* data = prefs.data();
      size_t len = prefs.used();
      for (size_t i = 0; i < len; i++) {
        c->printf("Prefs[%04d] = %02x '%c'\n", i, data[i], data[i]);
      }
    }
};
PrefsCommand thePrefsCommand;

class TimersCommand : public Command {
  public:
    const char* getName() { return "timers"; }
    const char* getHelp() { return "Print Timer Info"; }
    void execute(Console* c, uint8_t paramCount, char** params) {
      Timer::printInfo(c);
    }
};

TimersCommand theTimersCommand;

extern EventQueue* events;

class EventsCommand : public Command {
  public:
    const char* getName() { return "events"; }
    const char* getHelp() { return "display keyboard events"; }
    void execute(Console* c, uint8_t paramCount, char** params) {
      events->printStatus(c);
    }
};

EventsCommand theEventsCommand;

class QuitCommand : public Command {
  const char* getName() { return "quit"; }
  const char* getHelp() { return "exit the current app"; }
  void execute(Console* c, uint8_t paramCount, char** params) {
    launcher.exit();
  }
};
QuitCommand theQuitCommand;
