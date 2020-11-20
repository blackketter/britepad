#include "BritepadShared.h"

class AmbientWakeupApp : public BritepadApp {

  public:
    void init() {
      prefs.get(id(), sizeof(_ambientWakeupEnabled), &_ambientWakeupEnabled);
      resetAmbientWakeup();
    };

    void run() {
      _ambientWakeupEnabled = !_ambientWakeupEnabled;
      resetAmbientWakeup();
      prefs.set(id(), sizeof(_ambientWakeupEnabled), &_ambientWakeupEnabled);
   }

    const char* name() { return _ambientWakeupEnabled ? "Ambient Wakeup On" : "Ambient Wakeup Off"; };

    virtual bool highlighted() { return _ambientWakeupEnabled; }
    bool canBeInvisible() { return true; };
    AppType getAppType() { return SETTINGS_APP; }

    appid_t id() { return ID; };
    static constexpr appid_t ID = "ambw";

    void resetAmbientWakeup(millis_t defer = _ambientWakeupInterval );
    void enableAmbientWakeup(bool enabled) { enabled ? resetAmbientWakeup() : _ambientWakeupTimer.cancel(); };
    void ambientWakeupCallback();

  private:

    uint8_t _ambientWakeupEnabled;
    uint16_t _lastAmbient = 0;
    CallbackTimer _ambientWakeupTimer;
    static const millis_t _ambientWakeupInterval = 500;
    static const millis_t _ambientWakeupLongInterval = 10* 1000;  // don't wake up more than once a minute
    static const uint16_t _ambientChangeThreshold = 10;
};

AmbientWakeupApp theAmbientWakeupApp;

void ambientWakeupCallbackFunc(void* data) {
  ((AmbientWakeupApp*)data)->ambientWakeupCallback();
}

void AmbientWakeupApp::resetAmbientWakeup(millis_t defer) {
  if (_ambientWakeupEnabled) {
    _ambientWakeupTimer.setMillis(defer, ambientWakeupCallbackFunc, (void*)this);
 } else {
    _ambientWakeupTimer.cancel();
  }
}


void AmbientWakeupApp::ambientWakeupCallback() {

  uint16_t ambient = pad.getAmbientLight();
  millis_t waitTime = _ambientWakeupInterval;

  if (Uptime::millis() - britepad.lastEventTime() > _ambientWakeupLongInterval) {
    if (ambient > _lastAmbient && (ambient - _lastAmbient) > _ambientChangeThreshold) {
      britepad.wakeHost();
      console.debugln("Ambient Wakeup!");
      waitTime =_ambientWakeupLongInterval;
    }
  } else {
    resetAmbientWakeup();
  }

  resetAmbientWakeup(waitTime);
  _lastAmbient = ambient;
}
