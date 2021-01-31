#include "ScreensaverApp.h"

class Life1dApp : public ScreensaverApp {
  public:
    void run() override;
    void begin(AppMode asMode) override;
    void end() override { delete _world; }
    bool canBeMouse() override { return false; }

    const char* name() override { return "1D Life"; };
    appid_t id() override { return ID; };
    static constexpr appid_t ID = "1dlf";

  private:
    uint8_t* _world;
    coord_t _width;
    void seed();
    void newRules();
    uint8_t _rules[8];
    millis_t lastRefresh = 0;
};

Life1dApp theLife1dApp;

void Life1dApp::begin(AppMode asMode) {
  ScreensaverApp::begin(asMode);
  _width = screen.clipWidth();
  _world = new uint8_t[_width];
  seed();
  newRules();
}

void Life1dApp::newRules() {
  for (int i = 0; i < 8; i++) {
    _rules[i] = random(2);
  }
}

void Life1dApp::seed() {
  for (coord_t i = 0; i < _width; i++) {
    _world[i] = 0;
  }

  switch (random(3)) {
    case 0:
     _world[_width/2] = 1;
    break;

    case 1:
      for (int i = 0; i < _width; i++) {
        _world[i] = random(2);
      }
    break;

    case 2:
      int c = 10;
      for (int i = 0; i < c; i++) {
        _world[random(_width)] = 1;
      }
    break;
  }
  _world[_width/2] = 1;
}

void Life1dApp::run() {

  ScreensaverApp::run();

  coord_t t = screen.clipTop();
  coord_t b = screen.clipBottom();
  coord_t l = screen.clipLeft();

 // new rules every 5 seconds
  if (Uptime::millis() - lastRefresh > 3000) {
    newRules();
    lastRefresh = Uptime::millis();
  }

  seed();

  uint8_t newWorld[_width];
  newWorld[0] = 0;
  newWorld[_width-1] = 0;

  color_t drawWorld[_width];
  drawWorld[0] = Screen::black;
  drawWorld[_width-1] = Screen::black;

  for (int y = t; y < b; y++) {
    for (int x = 1; x < _width-1; x++) {
      uint8_t index = (_world[x-1] << 2) + (_world[x] << 1) + _world[x+1];
      newWorld[x] = _rules[index];
      drawWorld[x] = newWorld[x] ? Screen::white : Screen::black;
    }

    screen.drawFastHLine(l, y, _width, drawWorld);

    for (int x = 0; x < _width; x++) {
      _world[x] = newWorld[x];
    }

    launcher.idle();
  }
}
