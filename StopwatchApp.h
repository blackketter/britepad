#ifndef _StopwatchApp_
#define _StopwatchApp_

#include "BritepadApp.h"
#include "Button.h"

class StopwatchApp : public BritepadApp {
  public:
    void run();
    void begin();
    void setAppMode(AppMode asMode);

    const char* name() { return "Stopwatch"; };
    bool displaysStatusBar() { return true; }
    bool wantsToBeScreensaver() { return !isReset(); }
    bool disablesScreensavers() { return isAppMode(INTERACTIVE) && wantsToBeScreensaver(); }
    bool canBeInteractive() { return true; }

    appid_t id() { return ID; };
    static constexpr appid_t ID = "stpw";

  private:
    virtual bool isPaused() { return startMillis < 0; };
    virtual void pause() { startMillis = -(clock.millis() - startMillis); }
    virtual void resume() { startMillis = clock.millis() + startMillis; }
    virtual void reset() { startMillis = -1; }
    virtual bool isReset() { return (startMillis == -1); }
    virtual void drawTime();

    void redrawTime() { lastDrawMillis = 0; };

    void redrawButtons();
    long startMillis = -1;  // start out paused at zero(ish)
    RoundButton pauseButton;
    RoundButton resetButton;

  protected:
    millis_t lastDrawMillis;

};

#endif


