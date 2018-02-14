#include "BritepadShared.h"
#include "Britepad.h"
#include "apps/BritepadApp.h"

class AppsCommand : public Command {
  public:
    const char* getName() { return "apps"; }
    const char* getHelp() { return "Lists installed apps"; }
    void execute(Stream* c, uint8_t paramCount, char** params) {
        // show the apps that have been loaded
        BritepadApp* anApp = britepad.getNextApp();
        int count = 1;
        while (anApp != nullptr) {
          c->printf("  %d : %s - %s (%08x)\n", count, anApp->id(), anApp->name(), (uint32_t)anApp);
          anApp = anApp->getNextApp();
          count++;
        }
    }
};
AppsCommand theAppsCommand;

BritepadApp* findApp(const char* s) {
   BritepadApp* a = nullptr;
    int n = atoi(s);
    if (n) {
      a = britepad.getNextApp();
      while (n > 1) {
        a = britepad.getNextApp(a);
        n--;
      }
    } else {
        a = britepad.getAppByID(s);
    }
  return a;
}


class AppCommand : public Command {
  public:
    const char* getName() { return "app"; }
    const char* getHelp() { return "app (appid | index) - Info about an app, defaults to current app"; }
    void execute(Stream* c, uint8_t paramCount, char** params) {
      BritepadApp* anApp = nullptr;
      if (paramCount) {
        anApp = findApp(params[1]);
      }
      if (anApp == nullptr) {
        anApp = britepad.currentApp();
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
            modeString.append(anApp->getAppMode());
        };
        c->printf("  mode: %s\n", modeString.c_str());
      } else {
        c->printf("No currently running app!\n");
      }
    }
};
AppCommand theAppCommand;

class RunCommand : public Command {
  public:
    const char* getName() { return "run"; }
    const char* getHelp() { return "appid | index - Run app given id or index"; }
    void execute(Stream* c, uint8_t paramCount, char** params) {
        BritepadApp* a = findApp(params[1]);

        if (a) {
          c->printf("Running app '%s' - %s...\n",a->id(), a->name());
          britepad.launchApp(a);
          britepad.resetScreensaver(5*60*1000);  // run it for 5 minutes, then let the screensavers do their work
        } else {
          c->println("Invalid app id");
          printError(c);
        }
      }
};
RunCommand theRunCommand;

class PrefsCommand : public Command {
  public:
    const char* getName() { return "prefs"; }
    const char* getHelp() { return "dump prefs data"; }
    void execute(Stream* c, uint8_t paramCount, char** params) {
      const uint8_t* data = prefs.data();
      size_t len = prefs.used();
      for (size_t i = 0; i < len; i++) {
        c->printf("Prefs[%04d] = %02x '%c'\n", i, data[i], data[i]);
      }
    }
};
PrefsCommand thePrefsCommand;
