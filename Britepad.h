#ifndef _Britepad_
#define _Britepad_

#include "BritepadShared.h"
#include "Timer.h"

class BritepadApp;
class LauncherApp;

class Britepad {
  public:
    Britepad();

    void begin();
    void idle();

    void setNextApp(BritepadApp* app, AppMode mode = INTERACTIVE) { nextApp = app; nextAppMode = mode; };
    BritepadApp* getNextApp() { return nextApp; }
    AppMode getNextAppMode() { return nextAppMode; }

    void addApp(BritepadApp* newApp);
    int appsAdded() { return appCount; };
    BritepadApp* getApp(int appIndex);
    BritepadApp* getApp(appid_t appID);
    BritepadApp* currentApp() { return currApp; }
    void updateStatusBar();

  private:
    void setApp(BritepadApp* newApp, AppMode asMode);  // sets the current app

    BritepadApp* wantsToBeScreensaver();
    BritepadApp* randomApp(AppMode m);


    int appCount = 0;
    static const int maxApps = 100;
    static const millis_t screensaverDelay = 5000;
    static const millis_t screensaverSwitchInterval = 30000;
    static const millis_t ambientUpdateInterval = 100;

    static const millis_t checkWantsToBeScreensaverInterval = 1000;
    millis_t lastCheckWantsToBeScreensaver = 0;

    Timer     backlightTimer;
    Timer     statusBarUpdateTimer;

    millis_t screensaverStartedTime = 0;

    BritepadApp* apps[maxApps];

    BritepadApp* currApp = nullptr;
    BritepadApp* nextApp = nullptr;
    AppMode nextAppMode = INTERACTIVE;
};

#endif
