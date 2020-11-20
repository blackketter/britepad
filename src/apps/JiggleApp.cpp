#include "BritepadApp.h"
#include "BritepadShared.h"

class JiggleApp : public BritepadApp {

  public:
    void init() {
      prefs.get(id(), sizeof(_jiggleEnabled), &_jiggleEnabled);
      resetJiggle();
    };

    void run() {
      _jiggleEnabled = !_jiggleEnabled;
      resetJiggle();
      prefs.set(id(), sizeof(_jiggleEnabled), &_jiggleEnabled);
   }

    const char* name() { return _jiggleEnabled ? "Jiggle On" : "Jiggle Off"; };

    virtual bool highlighted() { return _jiggleEnabled; }
    bool canBeInvisible() { return true; };
    AppType getAppType() { return SETTINGS_APP; }

    appid_t id() { return ID; };
    static constexpr appid_t ID = "jigg";

    void resetJiggle();
    void enableJiggle(bool enabled) { enabled ? resetJiggle() : _jiggleTimer.cancel(); };
    void jigglerCallback();

  private:

    uint8_t _jiggleEnabled;
    CallbackTimer _jiggleTimer;
    static const millis_t _jiggleInterval = 10000;
};

JiggleApp theJiggleApp;

void jiggleCallback(void* data) {
  ((JiggleApp*)data)->jigglerCallback();
}

void JiggleApp::resetJiggle() {
  if (_jiggleEnabled) {
    _jiggleTimer.setMillis(_jiggleInterval, jiggleCallback, (void*)this);
 } else {
    _jiggleTimer.cancel();
  }
}


void JiggleApp::jigglerCallback() {
  console.debugln("Jiggle");
  Mouse.move(-1, 0);
  Mouse.move(1, 0);
  resetJiggle();
}
