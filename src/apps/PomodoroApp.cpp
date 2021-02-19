#include "BritepadShared.h"
#include "BritepadApp.h"
#include "Clock.h"
#include "Timer.h"
#include "widgets/Button.h"

class PomodoroApp : public BritepadApp {
  public:
    PomodoroApp(int pos) : BritepadApp() { setLauncherPosition(pos); }
    virtual void begin();
    virtual void run();

    bool wantsToRun() { return isRunning(); }
    bool canBeScreensaver() { return isRunning(); }
    virtual bool canBeMouse() { return isRunning(); }
    BritepadApp* exitsTo() override { return isAppMode(INTERACTIVE_MODE) ? EXIT_APP : SWITCH_TO_INTERACTIVE_MODE; }
    void setAppMode(AppMode asMode);

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
    CallbackTimer _timer;
    RoundButton _button;
    bool _isWorking = false;
};
PomodoroApp thePomodoroApp(10);

void PomodoroApp::begin() {
  BritepadApp::begin();
  if (getAppMode() == INTERACTIVE_MODE && !isRunning()) {
    alarm();  // auto-start
  }
  drawButton();
  drawTime();
}

void PomodoroApp::setAppMode(AppMode asMode) {
  BritepadApp::setAppMode(asMode);
  clearScreen();
  drawButton();
  drawTime();
}

void PomodoroApp::run() {
  BritepadApp::run();

  bool drawTimeNow = false;
  if (Uptime::millis() - _lastDraw > _redrawInterval) {
    drawTimeNow = true;
  }

  if (_button.pressed()) {
    sound.click();

    if (isRunning()) {
      // reset
      reset();
      _button.setColor(screen.green);
      _button.setTitle("Start");
      _button.draw();
    } else {
      // start
      alarm();
      _button.setColor(screen.red);
      _button.setTitle("Stop");
    }
    drawTimeNow = true;
    drawButton();
  }

  if (drawTimeNow) {
    // drawText
    drawTime();
  }
}

void PomodoroApp::drawButton() {
  coord_t y;
  coord_t radius = screen.clipHeight()/10;
  if (isAppMode(INTERACTIVE_MODE)) {
    y = screen.clipTop()+(screen.clipHeight()/6)*5;
  } else {
    y = screen.clipBottom()+radius;  // todo make this visible and enable bottom bar switching to interactive mode
  }

  _button.init(screen.clipMidWidth(), y, radius,
    isRunning() ? screen.red : screen.green, false,
    isRunning() ? "Stop" : "Start");
  _button.draw();
}

void PomodoroApp::drawTime() {

  _lastDraw = Uptime::millis();
  char textTime[10];

  color_t textColor = screen.white;
  const char* label = "Stopped";
  if (isWorking()) {
    textColor = screen.green;
    label = "     Go!     ";
  }
  if (isResting()) {
    textColor = screen.blue;
    label = "    Rest    ";
  }

  screen.setTextColor(textColor, bgColor());

  int remaining = isRunning() ? _timer.remainingSecs() : _workInterval;

  screen.setFont(&Arial_72_Digits_Bold);
  sprintf(textTime, " %d:%02d ", remaining/60, remaining%60);
  coord_t width = screen.measureTextWidth(textTime);

  screen.setCursor(screen.clipMidWidth() - width/2,
                   screen.clipTop() + screen.clipHeight()/2 - screen.measureTextHeight(textTime)/2);

  rect_t clip = { screen.getCursorX(), screen.getCursorY(),(coord_t)screen.measureTextWidth(textTime),(coord_t)(screen.measureTextHeight(textTime)+2)};
  screen.pushClipRect(&clip);
  screen.drawText(textTime);
  screen.pushClipRect(&clip);

  screen.setFont(&Arial_20_Bold);
  screen.setCursor(screen.clipMidWidth() - screen.measureTextWidth(label)/2,
                   screen.clipTop() + screen.clipHeight()/6 - screen.measureTextHeight(label)/2);

  screen.drawText(label);
}

void pomocallback(void* data) {
  ((PomodoroApp*)data)->alarm();
}

void PomodoroApp::alarm() {
  sound.bell(3000, 2096);
  _isWorking = !_isWorking;
  _timer.setSecs(_isWorking ? _workInterval : _restInterval, pomocallback, (void*)this);
}

void PomodoroApp::reset() {
  _timer.cancel();
  _isWorking = false;
}

