#ifndef _Britepad_
#define _Britepad_
#include "apps/BritepadApp.h"
#include "Launcher.h"

class BritepadLauncher : public Launcher {
  public:
    void begin() override;  // set up launcher
    void run() override;   // event loop for current app (also idles)
    void idle() override;   // system idle

    BritepadApp* currentBritepadApp() { return (BritepadApp*)currentApp(); }

    void launchApp(App* app) override;
    void launchApp(appid_t app) override;

    void launchApp(BritepadApp* app, AppMode mode);
    void launchApp(appid_t id, AppMode mode);

    void exit(); // exits current app

    AppMode getLaunchedAppMode() { return _launchedAppMode; }

    void drawBars(bool update = false); // draw the status and info bars
    void updateStatusBar();

// BritepadLauncher launches screensavers
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

// TODO: move these out
    static const int minBrightness = 20;
    void wakeHost();

  protected:
    millis_t disableScreensaversUntil = 0;
    millis_t screensaverStartedTime = 0;
    BritepadApp* wantsToRun();

  private:
    void drawInfoBar(bool update = false);
    void drawStatusBar(bool update);

    void resetClipRect();

    BritepadApp* randomApp(AppMode m);

    AppMode _launchedAppMode = INTERACTIVE_MODE;
    AppMode _lastAppMode = INTERACTIVE_MODE;

    static const time_t defaultScreensaverSwitchInterval = 30;
    static const time_t defaultScreensaverStartInterval = 10;

    const char* screensaverSwitchIntervalPref = "scri";
    const char* screensaverStartIntervalPref = "ssst";

    static const millis_t ambientUpdateInterval = 100;
    static const millis_t showClockDur = 2000;
    static const coord_t _statusBarHeight = 16;
    CallbackTimer     statusBarUpdateTimer;
    CallbackTimer     backlightTimer;

    millis_t lastIdle = 0;
    millis_t idleInterval = 8;  // minimum interval between idles.  8ms is 125hz or the default mouse speed

};

#endif
