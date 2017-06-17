#ifndef _PomodoroApp_
#define _PomodoroApp_

#include "ScreensaverApp.h"
#include "BritepadShared.h"
#include "Timer.h"
#include "StopwatchApp.h"

class PomodoroApp : public BritepadApp {
  public:
    PomodoroApp(int pos) : BritepadApp() { setLauncherPosition(pos); }
    virtual void begin(AppMode asMode);
    virtual void run();

    bool wantsToRun() { return isRunning(); }
    bool canBeScreensaver() { return isRunning(); }
//    BritepadApp* exitsTo() { return isAppMode(INTERACTIVE_MODE) ? EXIT_APP : STAY_IN_APP; }
//    bool disablesScreensavers() { return isAppMode(INTERACTIVE_MODE) && wantsToRun(); }
    void switchAppMode(AppMode asMode);

    AppType getAppType() { return TIMER_APP; }

    const char* name() { return "Pomodoro"; };
    appid_t id() { return ID; };
    static constexpr appid_t ID = "pomo";

    void alarm();

    color_t appButtonColor() { return screen.red; }

  private:
    bool isRunning() { return _timer.isRunning(); }  // are we counting down?
    bool isWorking() { return isRunning() && _isWorking; }
    bool isResting() { return isRunning() && !_isWorking; }

    virtual void reset();

    void drawTime();
    void drawButton();

    const time_t _workInterval = 25*60;
    const time_t _restInterval = 5*60;

    const millis_t _redrawInterval = 1000;

    millis_t _lastDraw = 0;
    Timer _timer;
    RoundButton _button;
    bool _isWorking = false;
    bool _firstRun;
};

#endif


