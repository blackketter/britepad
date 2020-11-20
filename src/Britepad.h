#ifndef _Britepad_
#define _Britepad_
#include "BritepadShared.h"
#include "apps/BritepadApp.h"

class Britepad {
  public:
    void begin();  // system setup
    void loop();   // event loop
    void idle();   // background processing (esp. for keyboard event handling)

    void launchApp(BritepadApp* app, AppMode mode = INTERACTIVE_MODE);
    void launchApp(appid_t id, AppMode mode = INTERACTIVE_MODE);
    BritepadApp* currentApp() { return _currApp; }
    void exit();

    void updateStatusBar();
    BritepadApp* getLaunchedApp() { return launchedAppPtr; }
    AppMode getLaunchedAppMode() { return launchedAppMode; }

    static const int minBrightness = 20;

    time_t getScreensaverSwitchInterval();
    void setScreensaverSwitchInterval(time_t newInterval);

    time_t getScreensaverStartInterval();
    void setScreensaverStartInterval(time_t newInterval);

    void resetScreensaver(millis_t dur = 0) { if (dur == 0) { dur = getScreensaverStartInterval()*1000; } disableScreensaversUntil = Uptime::millis() + dur; }

    bool event(KeyEvent* e);
    void timeChanged();

    void wakeHost();
    millis_t lastEventTime() { return keyEvents.lastEventTime(); }

    void drawInfoBar(bool update = false);
    void drawBars(bool update = false); // draw the status and info bars
    void drawStatusBar(bool update);

  private:
    BritepadApp* _currApp = nullptr;
    BritepadApp* launchedAppPtr = nullptr;
    AppMode launchedAppMode = INTERACTIVE_MODE;

    BritepadApp* lastAppPtr = nullptr;
    AppMode lastAppMode = INTERACTIVE_MODE;

    void setApp(BritepadApp* newApp, AppMode asMode);  // sets the current app
    void setCurrentApp(BritepadApp* newApp) { _currApp = newApp; };

    BritepadApp* wantsToRun();
    BritepadApp* randomApp(AppMode m);

    void resetClipRect();

    millis_t lastIdle = 0;
    millis_t idleInterval = 8;  // minimum interval between idles.  8ms is 125hz or the default mouse speed

    static const millis_t ambientUpdateInterval = 100;
    static const millis_t showClockDur = 2000;

    static const time_t defaultScreensaverSwitchInterval = 30;
    static const time_t defaultScreensaverStartInterval = 10;

    CallbackTimer     backlightTimer;

    CallbackTimer     statusBarUpdateTimer;

    millis_t screensaverStartedTime = 0;
    millis_t disableScreensaversUntil = 0;

    const char* screensaverSwitchIntervalPref = "scri";
    const char* screensaverStartIntervalPref = "ssst";

    static const coord_t _statusBarHeight = 16;
};

#endif
