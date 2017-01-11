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

