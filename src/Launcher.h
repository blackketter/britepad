#ifndef _Launcher_
#define _Launcher_
#include "App.h"
#include "Console.h"
#include "KeyEventQueue.h"
extern KeyEventQueue* keyEvents;

class Launcher {
  public:
    virtual void begin();  // system setup
    virtual void loop();   // event loop
    virtual void idle();   // background processing (esp. for keyboard event handling)

    void launchApp(App* app, AppMode mode = INTERACTIVE_MODE);
    void launchApp(appid_t id, AppMode mode = INTERACTIVE_MODE);
    App* currentApp() { return _currApp; }
    void exit();
    time_t getScreensaverSwitchInterval();
    void setScreensaverSwitchInterval(time_t newInterval);

    time_t getScreensaverStartInterval();
    void setScreensaverStartInterval(time_t newInterval);

    void resetScreensaver(millis_t dur = 0) {
      if (dur == 0) {
        dur = getScreensaverStartInterval()*1000;
      }
      disableScreensaversUntil = Uptime::millis() + dur;
    }

    bool event(KeyEvent* e);
    void timeChanged();

    virtual void wakeHost() {}; // TODO - move out
    millis_t lastEventTime() { return keyEvents->lastEventTime(); }

    App* getLaunchedApp() { return launchedAppPtr; }
    AppMode getLaunchedAppMode() { return launchedAppMode; }

  protected:
    virtual bool setApp(App* newApp, AppMode asMode);  // sets the current app, returns true if new app or mode is set
    App* wantsToRun();
    millis_t disableScreensaversUntil = 0;
    millis_t screensaverStartedTime = 0;

  private:
    App* _currApp = nullptr;
    App* launchedAppPtr = nullptr;
    AppMode launchedAppMode = INTERACTIVE_MODE;

    App* lastAppPtr = nullptr;
    AppMode lastAppMode = INTERACTIVE_MODE;

    void setCurrentApp(App* newApp) { _currApp = newApp; };

    App* randomApp(AppMode m);

    millis_t lastIdle = 0;
    millis_t idleInterval = 8;  // minimum interval between idles.  8ms is 125hz or the default mouse speed

    static const time_t defaultScreensaverSwitchInterval = 30;
    static const time_t defaultScreensaverStartInterval = 10;

    const char* screensaverSwitchIntervalPref = "scri";
    const char* screensaverStartIntervalPref = "ssst";

};
#endif
