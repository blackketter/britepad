#include "BritepadApp.h"
#include "widgets/Button.h"

class BellApp : public BritepadApp {

  public:
    void begin(AppMode asMode);
    void run();
    void init();

    const char* name() { return name_str; };

    bool disablesScreensavers() { return true; }

    appid_t id() { return ID; };
    static constexpr appid_t ID = "bell";
    AppType getAppType() { return SETTINGS_APP; }

    void bellCallback();

  private:

    const char* name_str = "Bell";

    int16_t _bell_interval = 20*60;  // 20 minutes
    bool _bell_enabled = true;

    void drawTime();
    void drawButtons();
    void resetTimer();

    static const int buttoncount = 5;
    time_t _last_run;

    CallbackTimer _bell_timer;

    RoundButton button[buttoncount];
};

BellApp theBellApp;

void bellCallbackFunc(void* data) {
  ((BellApp*)data)->bellCallback();
}

void BellApp::bellCallback() {
  sound.bell(3000, 1046);  // C6
  resetTimer();
}

void BellApp::resetTimer() {
    if (_bell_enabled) {
      _bell_timer.setSecs(_bell_interval, bellCallbackFunc, (void*)this);
    } else {
      _bell_timer.cancel();
    }
}

void BellApp::init() {
  int16_t prefdata;
  prefs.read(id(), sizeof(prefdata), (uint8_t*)&prefdata);
  _bell_enabled = (prefdata > 0) ? true : false;
  _bell_interval = abs(prefdata);
  resetTimer();
}

void BellApp::run() {

  stime_t adj = 0;
  if (button[0].pressed()) { adj = 60; }
  if (button[1].pressed()) { adj = -60; }

  if (button[2].pressed()) { adj = 1; }
  if (button[3].pressed()) { adj = -1; }

  _bell_interval += adj;

  if (_bell_interval < 5) { _bell_interval = 5; }
  if (_bell_interval > 99*60) { _bell_interval = 99*60; }

  if (button[4].pressed()) {
    adj = 1; // force an update of the timer
    _bell_enabled = !_bell_enabled;
    button[4].setTitle(_bell_enabled ? "On" : "Off");
    button[4].setColor(_bell_enabled ? screen.green : screen.darkgreen);
  }

  if (adj) {
    resetTimer();
    int16_t prefdata = _bell_enabled ? _bell_interval : -_bell_interval;
    prefs.write(id(), sizeof(prefdata), (uint8_t*)&prefdata);
  }

  if (adj || (_last_run != _bell_timer.remainingSecs())) {
    _last_run = _bell_timer.remainingSecs();
    drawTime();
  }

};


void BellApp::drawTime() {

    char timeStr[100];

    screen.setTextColor(screen.white, bgColor());
    screen.setFont(Arial_72_Digits_Bold);

    sprintf(timeStr, " %d:%02d ", _bell_interval/60, _bell_interval%60);

    screen.setCursor(screen.clipMidWidth() - screen.measureTextWidth(timeStr)/2,
                     screen.clipMidHeight() - screen.measureTextHeight(timeStr)/2);

    rect_t clip = { screen.getCursorX(), screen.getCursorY(),(coord_t)screen.measureTextWidth(timeStr),(coord_t)(screen.measureTextHeight(timeStr)+2)};
    screen.pushClipRect(&clip);
    screen.drawText(timeStr);
    screen.pushClipRect(&clip);

// Countdown info
    screen.setTextColor(_bell_enabled ? screen.green : screen.red, bgColor());
    screen.setFont(Arial_16_Digits_Bold);

    int16_t remain = _bell_enabled ? _bell_timer.remainingSecs()+1 : _bell_interval;

    sprintf(timeStr, " %d:%02d ", remain/60, remain%60);

    screen.setCursor(screen.clipMidWidth() - screen.measureTextWidth(timeStr)/2,
                     screen.clipTop() + screen.clipHeight()/6 - screen.measureTextHeight(timeStr)/2);

    clip = { screen.getCursorX(), screen.getCursorY(),(coord_t)screen.measureTextWidth(timeStr),(coord_t)(screen.measureTextHeight(timeStr)+2)};
    screen.pushClipRect(&clip);
    screen.drawText(timeStr);
    screen.pushClipRect(&clip);

}

void BellApp::drawButtons() {
  for (int i = 0; i < buttoncount; i++) {
    button[i].draw();
  }
}

void BellApp::begin(AppMode asMode) {
  BritepadApp::begin(asMode);

  clearScreen();

  int ytop = screen.clipTop() + screen.clipHeight()/6;
  int ybottom = screen.clipBottom() - screen.clipHeight()/6;

  // todo: fix hard coded pixels because the font is hard coded
  int x1 =  screen.clipMidWidth()/2;
  int x2 =  screen.clipMidWidth();
  int x3 =  x2 + x1;

  coord_t buttonradius = 24;
  button[0].init(x1, ytop, buttonradius,screen.red, false, screen.white, plusIcon);
  button[1].init(x1, ybottom, buttonradius,screen.red, false, screen.white, minusIcon);
  button[2].init(x3, ytop, buttonradius,screen.red, false, screen.white, plusIcon);
  button[3].init(x3, ybottom, buttonradius,screen.red, false,screen.white, minusIcon);

  button[4].init(x2, ybottom, buttonradius,_bell_enabled ? screen.green : screen.darkgreen, false, _bell_enabled ? "On" : "Off", Arial_12_Bold, screen.black);

  drawTime();
  drawButtons();
 }
