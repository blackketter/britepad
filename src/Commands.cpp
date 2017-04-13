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

class AppCommand : public Command {
  public:
    const char* getName() { return "app"; }
    const char* getHelp() { return "Info about currently running app"; }
    void execute(Stream* c, uint8_t paramCount, char** params) {
        // show the apps that have been loaded
        BritepadApp* anApp = britepad.currentApp();
        if (anApp) {
          String modeString;
          c->printf("Current app:\n  name: %s\n  id:   %s\n  addr: %08x\n", anApp->name(), anApp->id(), (uint32_t)anApp);
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
        BritepadApp* a = nullptr;
        int n = atoi(params[1]);
        if (params[1]) {
          if (n) {
            a = britepad.getNextApp();
            while (n > 1) {
              a = britepad.getNextApp(a);
              n--;
            }
          } else {
              a = britepad.getAppByID(params[1]);
          }
        }

        if (a) {
          c->printf("Running app '%s' - %s...\n",a->id(), a->name());
          britepad.launchApp(a);
          britepad.disableScreensavers();
        } else {
          c->println("Invalid app id");
        }
      }
};
RunCommand theRunCommand;

