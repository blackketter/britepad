#ifndef _Britepad_
#define _Britepad_

#include "BritepadShared.h"
#include "Timer.h"

enum AppMode {
  MOUSE_MODE = 1,
  INTERACTIVE_MODE = 2,
  SCREENSAVER_MODE = 4,
  INACTIVE_MODE = 8,
  ANY_MODE = 255
};

typedef const char* appid_t;

class BritepadApp;
class LauncherApp;

class Britepad {
  public:
    void begin();
    void idle();

    void launchApp(BritepadApp* app, AppMode mode = INTERACTIVE_MODE) { launchedAppPtr = app; launchedAppMode = mode; }
    BritepadApp* currentApp() { return currApp; }

    static void addApp(BritepadApp* newApp);
    BritepadApp* getAppByID(appid_t appID);
    BritepadApp* getNextApp(BritepadApp* anApp = nullptr);
    void updateStatusBar();
    BritepadApp* getLaunchedApp() { return launchedAppPtr; }
    AppMode getLaunchedAppMode() { return launchedAppMode; }
    void disableScreensavers(millis_t dur = 1000*60*5) { disableScreensaversUntil = pad.time() + dur; };  // disable screensavers for a default of 5 minutes, or as specified

    static const int minBrightness = 20;

    void resetChime();
    void enableChime(bool enabled) { enabled ? resetChime() : chimeTimer.cancel(); };
    void chimerCallback();

    time_t getScreensaverSwitchInterval();
    void setScreensaverSwitchInterval(time_t newInterval);

  private:
    BritepadApp* currApp = nullptr;
    BritepadApp* launchedAppPtr = nullptr;
    AppMode launchedAppMode = INTERACTIVE_MODE;

    void setApp(BritepadApp* newApp, AppMode asMode);  // sets the current app

    BritepadApp* wantsToBe(AppMode m);
    BritepadApp* randomApp(AppMode m);

    BritepadApp* theLauncherApp;

    static const millis_t screensaverDelay = 10000;
    static const millis_t ambientUpdateInterval = 100;
    static const millis_t showClockDur = 5000;

    static const time_t defaultScreensaverSwitchInterval = 30;

    static const millis_t checkWantsToBeScreensaverInterval = 1000;
    millis_t lastCheckWantsToBeScreensaver = 0;

    Timer     backlightTimer;

    Timer     statusBarUpdateTimer;

    millis_t screensaverStartedTime = 0;
    millis_t disableScreensaversUntil = 0;

    Timer chimeTimer;
    int chimesRemaining = 0;
    static const millis_t chimeInterval = 500;
    const char* screensaverSwitchIntervalPref = "scri";

};

#endif
