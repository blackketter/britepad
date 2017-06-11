#ifndef _Britepad_
#define _Britepad_

#include "BritepadShared.h"
#include "Timer.h"

// Apps run in particular modes
enum AppMode {
  MOUSE_MODE = 1,
  INTERACTIVE_MODE = 2,
  SCREENSAVER_MODE = 4,
  INACTIVE_MODE = 8,
  KEYBOARD_MODE = 16,
  INVISIBLE_MODE = 32,
  SETUP_MODE = 64,
  ANY_MODE = 255
};

typedef const char* appid_t;

class BritepadApp;
class LauncherApp;

class Britepad {
  public:
    void begin();  // system setup
    void loop();   // event loop
    void idle();   // background processing (esp. for keyboard event handling)

    void launchApp(BritepadApp* app, AppMode mode = INTERACTIVE_MODE);
    BritepadApp* currentApp() { return currApp; }

    static void addApp(BritepadApp* newApp);
    BritepadApp* getAppByID(appid_t appID);
    void sortApps();
    BritepadApp* getNextApp(BritepadApp* anApp = nullptr);
    void updateStatusBar();
    BritepadApp* getLaunchedApp() { return launchedAppPtr; }
    AppMode getLaunchedAppMode() { return launchedAppMode; }

    static const int minBrightness = 20;

    time_t getScreensaverSwitchInterval();
    void setScreensaverSwitchInterval(time_t newInterval);
    void resetScreensaver(millis_t dur = screensaverDelay) { disableScreensaversUntil = Uptime::millis() + dur; }
    void idleApps(KeyEvent* e);

  private:
    BritepadApp* currApp = nullptr;
    BritepadApp* launchedAppPtr = nullptr;
    AppMode launchedAppMode = INTERACTIVE_MODE;

    void setApp(BritepadApp* newApp, AppMode asMode);  // sets the current app

    BritepadApp* wantsToRun();
    BritepadApp* randomApp(AppMode m);

    millis_t lastIdle = 0;
    millis_t idleInterval = 10;  // minimum idle interval

    static const millis_t screensaverDelay = 10000;
    static const millis_t ambientUpdateInterval = 100;
    static const millis_t showClockDur = 2000;

    static const time_t defaultScreensaverSwitchInterval = 30;

    static const millis_t checkWantsToBeScreensaverInterval = 1000;
    millis_t lastCheckWantsToBeScreensaver = 0;

    Timer     backlightTimer;

    Timer     statusBarUpdateTimer;

    millis_t screensaverStartedTime = 0;
    millis_t disableScreensaversUntil = 0;

    const char* screensaverSwitchIntervalPref = "scri";
};

#endif
