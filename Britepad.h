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
    void disableScreensavers(millis_t dur = 1000*60*5) { disableScreensaversUntil = pad.time() + dur; };  // disable screensavers for a default of 5 minutes, or as specified

    static const int minBrightness = 20;

  private:
    BritepadApp* currApp = nullptr;
    BritepadApp* launchedAppPtr = nullptr;
    AppMode launchedAppMode = INTERACTIVE_MODE;

    void setApp(BritepadApp* newApp, AppMode asMode);  // sets the current app

    BritepadApp* wantsToBe(AppMode m);
    BritepadApp* randomApp(AppMode m);

    static const millis_t screensaverDelay = 10000;
    static const millis_t screensaverSwitchInterval = 30000;
    static const millis_t ambientUpdateInterval = 100;
    static const millis_t showClockDur = 5000;

    static const millis_t checkWantsToBeScreensaverInterval = 1000;
    millis_t lastCheckWantsToBeScreensaver = 0;

    Timer     backlightTimer;

    Timer     statusBarUpdateTimer;

    millis_t screensaverStartedTime = 0;
    millis_t disableScreensaversUntil = 0;

    BritepadApp* appList = nullptr;
};

#endif
