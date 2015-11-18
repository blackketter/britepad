#ifndef _Britepad_
#define _Britepad_

#include "BritepadShared.h"
#include "Timer.h"

class BritepadApp;
class LauncherApp;

class Britepad {
  public:
    void begin();
    void idle();

    void launchApp(BritepadApp* app, AppMode mode = INTERACTIVE_MODE) { launchedAppPtr = app; launchedAppMode = mode; }
    BritepadApp* currentApp() { return currApp; }

    void addApp(BritepadApp* newApp);
    BritepadApp* getAppByID(appid_t appID);
    BritepadApp* getNextApp(BritepadApp* anApp = nullptr);
    void updateStatusBar();
    BritepadApp* getLaunchedApp() { return launchedAppPtr; }
    AppMode getLaunchedAppMode() { return launchedAppMode; }

  private:
    BritepadApp* currApp = nullptr;
    BritepadApp* launchedAppPtr = nullptr;
    AppMode launchedAppMode = INTERACTIVE_MODE;

    void setApp(BritepadApp* newApp, AppMode asMode);  // sets the current app

    BritepadApp* wantsToBeScreensaver();
    BritepadApp* randomApp(AppMode m);

    static const millis_t screensaverDelay = 10000;
    static const millis_t screensaverSwitchInterval = 30000;
    static const millis_t ambientUpdateInterval = 100;

    static const millis_t checkWantsToBeScreensaverInterval = 1000;
    millis_t lastCheckWantsToBeScreensaver = 0;

    Timer     backlightTimer;

    Timer     statusBarUpdateTimer;

    millis_t screensaverStartedTime = 0;

    BritepadApp* appList = nullptr;
};

#endif
