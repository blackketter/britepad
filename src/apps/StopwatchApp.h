#ifndef _StopwatchApp_
#define _StopwatchApp_

#include "BritepadApp.h"
#include "widgets/Button.h"

class StopwatchApp : public BritepadApp {
  public:
    StopwatchApp();
    StopwatchApp(int32_t pos);

    void run();
    void begin(AppMode asMode);
    void switchAppMode(AppMode asMode);

    const char* name() { return "Stopwatch"; };
    bool displaysStatusBar() { return true; }
    bool wantsToRun() { return !isReset(); }
    BritepadApp* exitsTo() { return isAppMode(INTERACTIVE_MODE) ? BACK_APP : STAY_IN_APP; }

    virtual bool canBeScreensaver() { return isRunning(); }

    appid_t id() { return ID; };
    static constexpr appid_t ID = "stpw";

    AppType getAppType() { return TIMER_APP; }

  protected:
    virtual bool isPaused() { return startMillis < 0; };
    virtual void pause() { startMillis = -(Uptime::millis() - startMillis); }
    virtual void resume() { startMillis = Uptime::millis() + startMillis; }
    virtual void reset() { startMillis = -1; }
    virtual bool isReset() { return (startMillis == -1); }
    virtual bool isRunning() { return !isPaused() && !isReset(); };
    virtual void drawTime();

    void redrawTime() { lastDrawMillis = 0; };

    void redrawButtons();
    millis_t startMillis = -1;  // start out paused at zero(ish)
    RoundButton pauseButton;
    RoundButton resetButton;

    millis_t lastDrawMillis;
    bool firstRun;

};

#endif


