#ifndef _StopwatchApp_
#define _StopwatchApp_

#include "BritepadApp.h"
#include "Button.h"

class StopwatchApp : public BritepadApp {
  public:
    void run(void);
    void begin(AppMode asMode);
    const char* name(void) { return "Stopwatch"; };
    bool displaysStatusBar(void) { return true; }
    bool wantsToBeScreensaver(void) { return !isReset(); }
    bool disablesScreensavers(void) { return isAppMode(INTERACTIVE) && wantsToBeScreensaver(); }
    bool canBeInteractive() { return true; }

    appid_t id() { return ID; };
    static constexpr appid_t ID = "stpw";

  private:
    virtual bool isPaused() { return startMillis < 0; };
    virtual void pause(void) { startMillis = -(clock.millis() - startMillis); }
    virtual void resume(void) { startMillis = clock.millis() + startMillis; }
    virtual void reset(void) { startMillis = -1; }
    virtual bool isReset(void) { return (startMillis == -1); }
    virtual void drawTime(void);

    void redrawTime(void) { lastDrawMillis = 0; };

    void redrawButtons();
    long startMillis = -1;  // start out paused at zero(ish)
    RoundButton pauseButton;
    RoundButton resetButton;

  protected:
    millis_t lastDrawMillis;

};

#endif


